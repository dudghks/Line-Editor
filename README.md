# Terminal-based Line Editor
This is a line editor that runs in the terminal of any POSIX-compatible system.

# Commands
The document can be interacted using the following commands. Commands are case-sensitive and use the following syntax:

```COMMAND [args] ```

## INSERT
The `INSERT` command is used to write text lines to the document. If a line number is provided, then it inserts the text at that line. If not, then the text is appended to a new line at the end of the document. If no text is provided, an empty line is inserted.

```INSERT [optional line number] [optional text]```

## DELETE
The `DELETE` command deletes lines. It takes a starting line and an optional amount of lines to delete (1 if not provided) as parameters.

```DELETE [line number] [optional line count]```

## PRINT
The `PRINT` command is used to view the document with line numbers. It takes a starting and optional ending line numbers (inclusive) as parameters. If the ending line is not provided, it will print the entire document, starting from the line of the first argument. If no parameters are provided, it will print the entire document.

```PRINT [optional starting line] [optional ending line]```

## INFO
The `INFO` command prints information about the document. No parameters are taken.

```INFO```

## EOF
The `EOF` command is used to exit the program. No parameters are taken. Upon exiting, the contents of the document with numbered lines are printed.

```EOF```

## RENAME
The `RENAME` command is used to rename the document. 

```RENAME [new title]```

## SAVEFILE
The `SAVEFILE` command is used to write the document to a file. The name of the file is provided as an argument.

```SAVEFILE [file name]```

## OPENFILE
The `OPENFILE` command is used to read a text file into the editor. The file overwrites any exisitng text in the document.

```OPENFILE [file name]```

## NP_COMPILERUNC
The `NP_COMPILERUNC` command assumes that the document is an ANSI C program and takes no parameters. It saves the document as a temporary C file, compiles it with the gcc flags `ansi` `Wall` `Wextra` `Wpedantic` `Werror`, and runs the program (given the compilation succeeds). It does not run the program with any command line parameters.

```NP_COMPILERUNC```
