#!/usr/bin/python3

# Instead of using m4, which is everywhere but horribly tricky to use,
# we have our own template language.

from typing import List, Sequence, Dict, Optional
import os
import sys
import re


class ParseContext:
    def __init__(self, source_file: str, line_no: int = 0) -> None:
        self.name = source_file
        self.line_no = line_no
        self.line = ''
        self.context: Dict[str, Sequence[str]] = {}
    
    def copy(self, new_c: Optional[Dict[str, Sequence[str]]] = None) -> 'ParseContext':
        ret = ParseContext(self.name, self.line_no)
        ret.line = self.line
        ret.context = dict(self.context)
        if new_c:
            # Yes, arrays are replaced
            ret.context.update(new_c)
        return ret
    
    def get(self, name: str) -> Optional[str]:
        ret = self.context.get(name)
        if not ret:
            return None
        if len(ret) != 1:
            return None
        return ret[0]

    def __str__(self) -> str:
        return f"{self.name}:{self.line_no}"


class FilePart:
    """Part of the parsed file."""
    def __init__(self, cxt: ParseContext) -> None:
        self.cxt = cxt

    def generate(self) -> str:
        raise NotImplementedError

    def add_child(self, child: 'FilePart') -> None:
        raise NotImplementedError
    
    def add_raw_text(self, text: ParseContext) -> None:
        self.add_child(RawText(text))


class RawText(FilePart):
    """A bit of raw text to insert into the file."""
    def __init__(self, cxt: ParseContext) -> None:
        FilePart.__init__(self, cxt)
        self.generated = False
        self.text = cxt.line.rstrip()
    
    def add_child(self, child: FilePart) -> None:
        raise Exception('cannot contain children')

    def generate(self) -> str:
        return self.text + "\n"
    
    @staticmethod
    def mk(parent: FilePart, text: str) -> 'RawText':
        new_cxt = parent.cxt.copy()
        new_cxt.line = text
        ret = RawText(new_cxt)
        ret.generated = True
        return ret


class ParentPart(FilePart):
    """Contains multiple, ordered parts."""
    def __init__(self, cxt: ParseContext, children: Sequence[FilePart]) -> None:
        FilePart.__init__(self, cxt)
        self.children = list(children)
    
    def add_child(self, child: FilePart) -> None:
        self.children.append(child)

    def generate(self) -> str:
        ret = ''
        for item in self.children:
            ret += item.generate()
        return ret


class WrapperPart(ParentPart):
    """Two bits around children"""
    def __init__(
        self, cxt: ParseContext,
        first: FilePart, last: FilePart, children: Sequence[FilePart],
    ) -> None:
        ParentPart.__init__(self, cxt, children)
        self.first = first
        self.last = last
    
    def generate(self) -> str:
        return self.first.generate() + super().generate() + self.last.generate()


class FileStart(FilePart):
    """Start of a file."""
    def __init__(self, cxt: ParseContext, name: str) -> None:
        FilePart.__init__(self, cxt)
        self.name = name
    
    def add_child(self, child: FilePart) -> None:
        raise Exception('cannot add children')

    def generate(self) -> str:
        return f"""
#ifndef _FS_SHELL__CMD_{self.name}_
#define _FS_SHELL__CMD_{self.name}_

"""


class FileEnd(FilePart):
    """End of a file."""
    def __init__(self, cxt: ParseContext, name: str) -> None:
        FilePart.__init__(self, cxt)
        self.name = name
    
    def add_child(self, child: FilePart) -> None:
        raise Exception('cannot add children')

    def generate(self) -> str:
        return f"""
#endif /* _FS_SHELL__CMD_{self.name}_ */
"""


class MacroSection(FilePart):
    """Blocks of text that are enclosed with backslashes"""
    def __init__(self, cxt: ParseContext, name: str) -> None:
        FilePart.__init__(self, cxt)
        self.name = name
        self.text: List[RawText] = []

    def add_child(self, child: FilePart) -> None:
        if isinstance(child, RawText):
            self.text.append(child)
        else:
            raise ValueError(f"{child.cxt} :: cannot be added inside a code section ({self.cxt})")
    
    def generate(self) -> str:
        ret = ''
        for raw in self.text:
            for line in raw.text.splitlines():
                pos = line.find('//')
                if pos >= 0:
                    line = line[:pos] + '/*' + line[pos+2:] + '*/'
                ret += line + ' \\\n'
        if ret.endswith(' \\\n'):
            ret = ret[:-3]
        return ret


class CmdStep(FilePart):
    def __init__(self, cxt: ParseContext, is_virtual: bool) -> None:
        FilePart.__init__(self, cxt)
        enum_name = cxt.get("enum")
        command_name = cxt.get("name")
        self.is_first = False
        self.is_virtual = is_virtual
        if not enum_name:
            # print(cxt.context)
            raise ValueError(f"{cxt} :: no enum given")
        elif not is_virtual and not command_name:
            raise ValueError(f"{cxt} :: no name given for named step")
        elif is_virtual:
            command_name = None
        elif command_name == '<first>':
            # Very special case for one file.
            self.is_first = True
            command_name = ""
        self.enum_name = enum_name.upper().replace('-', '_')
        self.command_name = command_name
        if command_name is not None:
            self.cmd_var_name = "cmd_name_" + enum_name.lower().replace('-', '_')
        else:
            self.cmd_var_name = None
        self.globals: List[MacroSection] = []
        self.inits: List[MacroSection] = []
        self.args: List[MacroSection] = []
        self.cmds: List[MacroSection] = []
        self.requires_another_arg = False
        self.raw: List[RawText] = []
    
    def add_child(self, child: FilePart) -> None:
        if isinstance(child, RawText):
            self.raw.append(child)
        elif isinstance(child, MacroSection):
            if child.name == 'GlobalDefs':
                self.globals.append(child)
            elif child.name == 'OnInit':
                self.inits.append(child)
            elif child.name == 'OnArg':
                self.args.append(child)
            elif child.name == 'OnCmd':
                self.cmds.append(child)
            elif child.name == 'RequiresAnotherArg':
                self.requires_another_arg = True
            else:
                raise ValueError(f"{child.cxt} :: cannot add `{child.name}` inside a command step ({self.cxt})")
        else:
            raise ValueError(f"{child.cxt} :: cannot be added inside a command step ({self.cxt})")


class StepCollection(FilePart):
    """All the steps joined together for a command."""
    def __init__(self, cxt: ParseContext, command_name: str) -> None:
        FilePart.__init__(self, cxt)
        self.command_name = command_name
        self.pre_steps: List[MacroSection] = []
        self.steps: List[CmdStep] = []
        self.references: List[str] = []

    def add_child(self, child: FilePart) -> None:
        if isinstance(child, MacroSection) and child.name == 'OnInit':
            self.pre_steps.append(child)
        elif isinstance(child, CmdStep):
            self.steps.append(child)
        else:
            raise Exception(f"Can't add {child} to a joined enum.")
    
    def generate(self) -> str:
        # print(f"StepCollection: references = {self.references}")
        ret = ''
        for step in self.steps:
            was_blank = True
            for raw in step.raw:
                val = raw.generate()
                if val.strip():
                    was_blank = False
                    ret += '\n' + val
                elif not was_blank:
                    was_blank = True
                    ret += '\n'
        for step in self.steps:
            if not step.is_virtual and step.cmd_var_name and step.command_name is not None:
                ret += f'/* from {step.cxt} */\n'
                ret += f'extern const char {step.cmd_var_name}[];'
        ret += f'\n#define ENUM_LIST__{self.command_name}'
        for ref in self.references:
            ret += f' \\\n            ENUM_LIST__{ref}'
        for step in self.steps:
            if not step.is_virtual:
                ret += f' \\\n            /* from {step.cxt} */'
                ret += f' \\\n            COMMAND_INDEX__{step.enum_name}'
                if step.is_first:
                    ret += ' = 0'
                ret += ','
        ret += f'\n#define VIRTUAL_ENUM_LIST__{self.command_name}'
        for ref in self.references:
            ret += f' \\\n            VIRTUAL_ENUM_LIST__{ref}'
        for step in self.steps:
            if step.is_virtual:
                ret += f' \\\n            /* from {step.cxt} */'
                ret += f' \\\n            COMMAND_INDEX__{step.enum_name},'
        ret += f'\n#define GLOBAL_VARDEF__{self.command_name}'
        for ref in self.references:
            ret += f' \\\n            GLOBAL_VARDEF__{ref}'
        for step in self.steps:
            if not step.is_virtual and step.cmd_var_name and step.command_name is not None:
                ret += f' \\\n            /* from {step.cxt} */'
                ret += f' \\\n            const char {step.cmd_var_name}[] = "{step.command_name}";'
            for mcr in step.globals:
                ret += f' \\\n            /* from {mcr.cxt} */'
                ret += f' \\\n{mcr.generate()}'
        ret += f'\n#define INITIALIZE__{self.command_name}'
        for ref in self.references:
            ret += f' \\\n            INITIALIZE__{ref}'
        for mcr in self.pre_steps:
            ret += f' \\\n            /* from {mcr.cxt} */'
            ret += f' \\\n{mcr.generate()}'
        for step in self.steps:
            if step.cmd_var_name:
                ret += f' \\\n            /* from {step.cxt} */'
                ret += f' \\\n            command_list_names[COMMAND_INDEX__{step.enum_name}] = {step.cmd_var_name};'
            for mcr in step.inits:
                ret += f' \\\n            /* from {mcr.cxt} */'
                ret += f' \\\n{mcr.generate()}'
        ret += f'\n#define STARTUP_CASE__{self.command_name}'
        for ref in self.references:
            ret += f' \\\n            STARTUP_CASE__{ref}'
        for step in self.steps:
            if step.cmds:
                ret += f' \\\n    case COMMAND_INDEX__{step.enum_name}:'
                ret += f' \\\n        /* from {step.cxt} */'
            for mcr in step.cmds:
                ret += f' \\\n            /* from {mcr.cxt} */'
                ret += f' \\\n{mcr.generate()}'
            if step.cmds:
                ret += f' \\\n        break;'
        ret += f'\n#define RUN_CASE__{self.command_name}'
        for ref in self.references:
            ret += f' \\\n            RUN_CASE__{ref}'
        for step in self.steps:
            if step.args:
                ret += f' \\\n    case COMMAND_INDEX__{step.enum_name}:'
                ret += f' \\\n        /* from {step.cxt} */'
            for mcr in step.args:
                ret += f' \\\n            /* from {mcr.cxt} */'
                ret += f' \\\n{mcr.generate()}'
            if step.args:
                ret += f' \\\n        break;'
        ret += f'\n#define REQUIRES_ADDL_ARG__{self.command_name}'
        for ref in self.references:
            ret += f' \\\n            REQUIRES_ADDL_ARG__{ref}'
        for step in self.steps:
            if step.requires_another_arg:
                ret += f' \\\n            case COMMAND_INDEX__{step.enum_name}:'
        ret += "\n"
        return ret


class CommandFileType(FilePart):
    """Describes if the command file is required, optional, or unified."""
    def __init__(self, cxt: ParseContext) -> None:
        FilePart.__init__(self, cxt)
        name = cxt.get('command')
        if not name:
            raise ValueError(f'{cxt} :: no `command` argument specified')
        self.name = name
        self.u_name = name.upper().replace('-', '_')
        self.has_file_wrapper = True
        self.children: List[FilePart] = []

    def generate(self) -> str:
        return self.complete().generate()
    
    def get_steps(self) -> Sequence[CmdStep]:
        ret: List[CmdStep] = []
        for kid in self.children:
            if isinstance(kid, CmdStep):
                ret.append(kid)
        return ret
    
    def add_child(self, child: FilePart) -> None:
        if isinstance(child, (RawText, CmdStep)):
            self.children.append(child)
        elif isinstance(child, MacroSection) and child.name == 'OnInit':
            self.children.append(child)
        else:
            raise ValueError(f'{child.cxt} :: cannot add inside a command definition ({self.cxt})')

    def complete(self) -> FilePart:
        """Turn this into a real file type."""
        ret: List[FilePart] = []
        if self.has_file_wrapper:
            return WrapperPart(
                self.cxt,
                FileStart(self.cxt, self.u_name), FileEnd(self.cxt, self.u_name),
                self._inner_complete(),
            )
        return ParentPart(self.cxt, self._inner_complete())
    
    def _inner_complete(self) -> Sequence[FilePart]:
        raise NotImplementedError

    def _join_steps(self) -> StepCollection:
        ret = StepCollection(self.cxt, self.u_name)
        for child in self.children:
            if isinstance(child, (CmdStep, MacroSection)):
                ret.add_child(child)
        return ret
    
    def _join_raw(self) -> Sequence[RawText]:
        ret: List[RawText] = []
        for child in self.children:
            if isinstance(child, RawText):
                ret.append(child)
        return ret


class RequiredCommandFile(CommandFileType):
    """A command file that must exist (can't be flagged off)."""
    def __init__(self, cxt: ParseContext) -> None:
        CommandFileType.__init__(self, cxt)
    
    def _inner_complete(self) -> Sequence[FilePart]:
        return [
            RawText.mk(self, f'// Required command {self.name}'),
            RawText.mk(self, ''),
            *self._join_raw(),
            self._join_steps(),
        ]


class OptionalCommandFile(CommandFileType):
    """A command file that is optionally included."""
    def __init__(self, cxt: ParseContext) -> None:
        CommandFileType.__init__(self, cxt)
        ifdef = cxt.get('ifdef')
        self.ifdef = ifdef or f'USE_CMD_{self.u_name}'

    def _inner_complete(self) -> Sequence[FilePart]:
        return [
            RawText.mk(self, f'// Optional command {self.name}'),
            RawText.mk(self, ''),
            RawText.mk(self, f'#ifdef {self.ifdef}'),
            RawText.mk(self, ''),
            *self._join_raw(),
            self._join_steps(),
            RawText.mk(self, ''),
            RawText.mk(self, f'#else /* {self.ifdef} */'),
            # empty step collection means just empty / unset values.
            StepCollection(self.cxt, self.u_name),
            RawText.mk(self, f'#endif /* {self.ifdef} */'),
        ]


class AuxCommandFilePart(FilePart):
    """A subset of commands that are included if any of the list of
    items are present."""
    def __init__(self, cxt: ParseContext) -> None:
        FilePart.__init__(self, cxt)
        enum_names = cxt.context.get('commands')
        if not enum_names:
            raise ValueError(f'{cxt} :: no `commands` argument specified')
        file_name = cxt.get('file')
        if not file_name:
            # print(cxt.context)
            raise ValueError(f'{cxt} :: No parent union declared for aux definition.')
        self.enum_names = enum_names
        self.u_names = [
            n.upper().replace('-', '_')
            for n in enum_names
        ]
        ifdefs = cxt.context.get('ifdefs')
        if not ifdefs:
            ifdefs = [
                f'USE_CMD_{e}'
                for e in self.u_names
            ]
        self.ifdefs = ifdefs
        # print(f"Aux referenced external commands: {self.u_names}")
        self.file_name = file_name
        self.u_file_name = file_name.upper().replace('-', '_')
        self.raw: List[RawText] = []
        self.steps: List[CmdStep] = []
    
    def add_child(self, child: 'FilePart') -> None:
        if isinstance(child, RawText):
            self.raw.append(child)
        elif isinstance(child, CmdStep):
            self.steps.append(child)
        else:
            raise ValueError(f'{self.cxt} :: invalid internal value at {child.cxt}')

    def generate(self) -> str:
        return self.complete().generate()

    def complete(self) -> FilePart:
        steps = StepCollection(self.cxt, self.u_file_name)
        steps.references = list(self.u_names)
        for child in self.steps:
            steps.add_child(child)
        parts: List[FilePart] = [
            RawText.mk(self, f'#if {self._mk_def()}'),
            RawText.mk(self, ''),
            *self.raw,
            steps,
            RawText.mk(self, ''),
            RawText.mk(self, f'#else /* {self._mk_def()} */'),
            # empty step collection means just empty / unset values.
            StepCollection(self.cxt, self.u_file_name),
            RawText.mk(self, f'#endif /* {self._mk_def()} */'),
        ]
        return ParentPart(self.cxt, parts)
    
    def _mk_def(self) -> str:
        return ' || '.join([
            f'defined({e})'
            for e in self.ifdefs
        ])



class OptionalUnionCommandFile(CommandFileType):
    """A command file that is optionally included, and consists of
    multiple commands."""
    def __init__(self, cxt: ParseContext) -> None:
        file_name = cxt.get("file")
        if not file_name:
            raise ValueError(f"{cxt} :: no `file` value given")
        repl_cxt = cxt.copy({"command": [file_name]})
        CommandFileType.__init__(self, repl_cxt)
    
    def add_child(self, child: FilePart) -> None:
        if isinstance(child, (RequiredCommandFile, OptionalCommandFile)):
            child.has_file_wrapper = False
            self.children.append(child)
        elif isinstance(child, AuxCommandFilePart):
            self.children.append(child)
        elif isinstance(child, RawText):
            CommandFileType.add_child(self, child)
        else:
            raise ValueError(f"{self.cxt} :: union files can only have other required, optional, and aux contents, found {child.cxt}")

    def _inner_complete(self) -> Sequence[FilePart]:
        return self.children


class CommandFile(FilePart):
    """The wrapper for the command file."""
    def __init__(self, cxt: ParseContext) -> None:
        FilePart.__init__(self, cxt)
        self.__children: List[FilePart] = []
        self.__cmd_added = False

    def add_child(self, child: FilePart) -> None:
        if isinstance(child, RawText):
            self.__children.append(child)
            return
        if isinstance(child, CommandFileType):
            if self.__cmd_added:
                raise Exception(f'{self.cxt} :: Files must have exactly 1 file type.')
            self.__cmd_added = True
            self.__children.append(child)
            return
        raise ValueError(f'{child.cxt} :: Sections of this type are not allowed in a command ({self.cxt})')
    
    def get_command_file_type(self) -> Optional[CommandFileType]:
        """Get the registered command file type."""
        for kid in self.__children:
            if isinstance(kid, CommandFileType):
                return kid
        return None

    def generate(self) -> str:
        ret = ''
        for item in self.__children:
            ret += item.generate()
        return ret



class ParseEntry:
    def __init__(self, container: FilePart, in_open: bool = False) -> None:
        self.container = container
        self.in_open_expr = in_open
        self.open_paren_count = 0
    
    def handle_open_entry(
            self, cxt: ParseContext,
            name: str, args: Dict[str, str],
    ) -> Optional['ParseEntry']:
        # TODO make this a bunch of factory methods.
        cct = cxt.copy()
        cct.context.update(self.container.cxt.context)
        cct.context.update(args)
        if name in ('AsOptional', 'WithOptional'):
            # print(f"{cxt} :: optional")
            child = OptionalCommandFile(cct)
            self.container.add_child(child)
            return ParseEntry(child, True)
        elif name in ('AsRequired', 'WithOptional'):
            # print(f"{cxt} :: required")
            child = RequiredCommandFile(cct)
            self.container.add_child(child)
            return ParseEntry(child, True)
        elif name == 'WithAuxFor':
            # print(f"{cxt} :: aux for")
            child = AuxCommandFilePart(cct)
            self.container.add_child(child)
            return ParseEntry(child, True)
        elif name == 'AsOptionalUnion':
            # print(f"{cxt} :: optional union")
            child = OptionalUnionCommandFile(cct)
            self.container.add_child(child)
            return ParseEntry(child, True)
        elif name == 'WithNamedStep':
            # print(f"{cxt} :: named step")
            child = CmdStep(cct, False)
            self.container.add_child(child)
            return ParseEntry(child, True)
        elif name == 'WithVirtualStep':
            # print(f"{cxt} :: virtual step")
            child = CmdStep(cct, True)
            self.container.add_child(child)
            return ParseEntry(child, True)
        elif name in ('OnInit', 'OnCmd', 'OnArg', 'GlobalDefs'):
            # print(f"{cxt} :: macro")
            child = MacroSection(cct, name)
            self.container.add_child(child)
            return ParseEntry(child, True)

        # Else no match; count as plain text
        self.container.add_raw_text(cxt.copy())
        return None
    
    def handle_closing(self, cxt: ParseContext) -> bool:
        if self.in_open_expr:
            self.in_open_expr = False
            # print(f"{cxt} :: closing context")
            return True
        return False
    
    def handle_complete_entry(
            self, cxt: ParseContext,
            name: str, args: Dict[str, str],
    ) -> None:
        cct = cxt.copy()
        cct.context.update(self.container.cxt.context)
        cct.context.update(args)
        if name == 'RequiresAnotherArg':
            child = MacroSection(cct, name)
            self.container.add_child(child)
        else:
            self.container.add_raw_text(cxt)


# Arguments can be in the form:
#   abc="def"
# or
#   abc=[abcd, efe-e, fef]
# The Key part is:
#   \w+\s*=\s*
# The String value part is:
#   "[^"]*"
# The array value part is:
#   \[\s*(?:[\w|-]+(?:\s*,\s*[\w|-]+)*)?\s*\]
# So the value match is:
#   (?:"[^"]*")|(?:\[\s*(?:[\w|-]+(?:\s*,\s*[\w|-]+)*)?\s*\])
# So a single argument is:
#   \w+\s*=\s*(?:(?:"[^"]*")|(?:\[\s*(?:[\w|-]+(?:\s*,\s*[\w|-]+)*)?\s*\]))
_INNER_NAMED_ARG = r'\w+\s*=\s*(?:(?:"[^"]*")|(?:\[\s*(?:[\w|-]+(?:\s*,\s*[\w|-]+)*)?\s*\]))'

OPEN_EXPR_LINE_RE = re.compile(r'^\s*(\w+)\s*\(((?:\s*' + _INNER_NAMED_ARG + r'\s*,)*)\s*$')
COMPLETE_EXPR_LINE_RE = re.compile(r'^\s*(\w+)\s*\(((?:\s*' + _INNER_NAMED_ARG + r'(?:\s*,\s*' + _INNER_NAMED_ARG + r')*)?\s*)\)\s*$')
CLOSE_EXPR_LINE_RE = re.compile(r'^\s*\)\s*$')
ARG_EXPR_RE = re.compile(f'({_INNER_NAMED_ARG})')
ARG_STR_PAIR_RE = re.compile(r'^\s*(\w+)\s*=\s*"([^"]*)"\s*$')
ARG_LIST_PAIR_RE = re.compile(r'^\s*(\w+)\s*=\s*\[\s*((?:[\w|-]+(?:\s*,\s*[\w|-]+)*)?)\s*\]\s*$')


def parse_args_as_context(args: str) -> Dict[str, Sequence[str]]:
    ret: Dict[str, Sequence[str]] = {}
    for arg_pairs in ARG_EXPR_RE.findall(args):
        mtc = ARG_STR_PAIR_RE.match(arg_pairs)
        if mtc:
            ret[mtc.group(1)] = (mtc.group(2),)
        else:
            mtc = ARG_LIST_PAIR_RE.match(arg_pairs)
            if not mtc:
                raise Exception(f'Bad parsing of [{args}] on [{arg_pairs}]')
            ret[mtc.group(1)] = tuple([
                t.strip()
                for t in mtc.group(2).split(',')
            ])
    return ret


def parse_source_file(source_file: str) -> CommandFile:
    """Parse the file."""
    cxt = ParseContext(source_file)
    stack: List[ParseEntry] = [ParseEntry(CommandFile(cxt))]

    with open(source_file, 'r', encoding='utf-8') as fis:
        # This is processed per line, to make our life easier.
        for line in fis.readlines():
            cxt.line_no += 1
            cxt.line = line
            # print(f"{cxt} :: parsing {line}")
            current = stack[-1]
            mtc = OPEN_EXPR_LINE_RE.match(line)
            if mtc:
                name = mtc.group(1)
                arg_list = parse_args_as_context(mtc.group(2))
                next_entry = current.handle_open_entry(cxt, name, arg_list)
                if next_entry:
                    # print(f" - entering {next_entry.container}")
                    next_entry.open_paren_count += 1
                    stack.append(next_entry)
                else:
                    current.open_paren_count += 1
                continue
            mtc = COMPLETE_EXPR_LINE_RE.match(line)
            if mtc:
                name = mtc.group(1)
                arg_list = parse_args_as_context(mtc.group(2))
                current.handle_complete_entry(cxt.copy(), name, arg_list)
                continue
            open_count = line.count('(')
            close_count = line.count(')')
            current.open_paren_count += open_count - close_count
            # print(f"{cxt} :: paren count {current.open_paren_count} <= {open_count} - {close_count}")
            if current.open_paren_count <= 0 and CLOSE_EXPR_LINE_RE.match(line):
                if current.handle_closing(cxt.copy()):
                    # Pop the current value.
                    if len(stack) <= 1:
                        raise Exception(
                            f"{cxt} :: Too many close parenthesis"
                        )
                    stack = stack[:-1]
                    # print(f" - current head is {stack[-1].container}")
                else:
                    # print(f"{cxt} :: could be closing, but current context says no")
                    current.container.add_raw_text(cxt.copy())
            else:
                current.container.add_raw_text(cxt.copy())
    if len(stack) != 1:
        raise Exception(
            f"{source_file}: Dangling parenthesis"
        )
    return stack[0].container



def generate_command_list(source_files: Sequence[str], suffix: str) -> str:
    """Load in all the source files and create the contents of the command list file."""
    enum_names: List[str] = []
    for source_file in source_files:
        source = parse_source_file(source_file + suffix)
        child = source.get_command_file_type()
        if child:
            enum_names.append(child.u_name)

    out_text = """// GENERATED FILE.  DO NOT MODIFY.

/*
MIT License

Copyright (c) 2022 groboclown

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, ERMPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _FS_SHELL__COMMAND_LIST_
#define _FS_SHELL__COMMAND_LIST_

"""
    for source_file in source_files:
        out_text += f'#include "{os.path.basename(source_file)}"\n'
    out_text += '\n'

    out_text += "\n#define CMD_GLOBAL_VARDEF"
    for name in enum_names:
        out_text += f" \\\n            GLOBAL_VARDEF__{name}"

    out_text += "\n#define CMD_INITIALIZE"
    for name in enum_names:
        out_text += f" \\\n            INITIALIZE__{name}"

    out_text += "\n#define CMD_STARTUP_CASE"
    for name in enum_names:
        out_text += f" \\\n            STARTUP_CASE__{name}"

    out_text += "\n#define CMD_RUN_CASE"
    for name in enum_names:
        out_text += f" \\\n            RUN_CASE__{name}"
    
    out_text += "\n#define CMD_REQUIRES_ADDL_ARG"
    for name in enum_names:
        out_text += f" \\\n            REQUIRES_ADDL_ARG__{name}"

    out_text += """

// All the commands, by index.
enum CommandIndex {

"""
    for name in enum_names:
        out_text += f"\n            ENUM_LIST__{name}"
    out_text += '\n\n    COMMAND_INDEX__LAST_NAMED_CMD,\n\n'

    for name in enum_names:
        out_text += f"\n            VIRTUAL_ENUM_LIST__{name}"
    out_text += '\n\n    COMMAND_INDEX__LAST_VIRTUAL_CMD\n};\n'

    out_text += '\n#endif /* _FS_SHELL__COMMAND_LIST_ */\n'
    return out_text


def process_command_list(source_files: Sequence[str], suffix: str, target_file: str) -> int:
    """Load in all the source files and turn them into a command list file."""
    contents = generate_command_list(source_files, suffix)
    with open(target_file, 'w', encoding='utf-8') as fos:
        fos.write(contents)
    return 0



def process_command_file(source_file: str, target_file: str) -> int:
    """Transform the source file into a command file."""
    parsed = parse_source_file(source_file)
    with open(target_file, 'w', encoding='utf-8') as fos:
        fos.write(f"// GENERATED FROM {source_file}.  DO NOT EDIT.\n\n")
        fos.write(parsed.generate())
    return 0


def main(args) -> int:
    """CLI entry."""
    if '-h' in args or '--help' in args:
        print(
            f"Usage: {args[0]} [--cmd-list (suffix)] (source file (...)) (target file)"
        )
        return 0
    if len(args) < 3:
        print("Too few arguments.  Run with --help to see how to run it.")
        return 1
    
    try:
        target_file = args[-1]
        if args[1] == '--cmd-list':
            if len(args) <= 5:
                print("Invalid command-list usage.  Run with --help to see how to run it.")
                return 1
            suffix = args[2]
            source_files = args[3:-1]
            return process_command_list(source_files, suffix, target_file)
        
        if len(args) != 3:
            print("Invalid single file usage.  Run with --help to see how to run it.")
            return 1
        source_file = args[1]
        if not os.path.isfile(source_file):
            print(f"Error: Could not find source file {source_file}")
            return 1
        return process_command_file(source_file, target_file)
    except ValueError as err:
        print(f"Error: {err}")
        return 1
    

if __name__ == '__main__':
    sys.exit(main(sys.argv))
