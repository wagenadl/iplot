// Command.h

#ifndef COMMAND_H

#define COMMAND_H

namespace Command {
  enum Command {
    None = 0,
    // COMMANDS
    Figure, // arg[0] may be [width, height]
    Close, 
    Axes, 
    Line, // handle not used
    Points, // handle not used
    Patch, // handle not used
    Subplot, // handle not used. arg[0] may be 1, 2, 3, or 4 integers
    Text, // arg[0] is [x,y], arg[1] is text
    Button, // arg[0] is type, arg[1] is text 
    Get, // arg[0] is key
    Set, // arg[0] is key, arg[1] is value
    Properties, // returns a space-separated list of properties
    Find, // arg[0] is tag
    Delete, // no args
    GCF, // handle not used
    GCA, // handle not used
    GCBO, // handle not used
    GCBF, // handle not used
    // RESPONSES
    OK, // sent in response to Set.
    Handle, // sent in response to Figure, Axes, Line, Points, Subplot
    Callback, // arg[0] is name of function to call
    Value, // sent in response to Get; arg[0] is key, arg[1] is value
    // RESPONSES THAT INDICATE ERRORS:
    BadCommand, // handle not used
    BadHandle, // handle returned
    BadKey, // handle and key in arg[0] as in corresponding Get/Set
    BadValue, // for Set or other commands. handle not used, no args. Also for
              // Get if IResponder doesn't know how to handle a QVariant.
              // (Obviously that's an internal error case.)
    BadSyntax, // e.g., incorrect number of arguments
  };
};

#endif
