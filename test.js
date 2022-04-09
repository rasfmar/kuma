





























/*

name = kuma
file_extensions [] = kma;
# background_color = #111116
background_color = #282a36
color = #ffccdd

contexts[] {
   main : context {
      # : pattern {
      #    regex \= (?:\()([a-zA-Z]+)(?:\))
      #   styles[] = .id;
      #}
      : inline_push {
         regex \= (\')
         styles[] = .string;
         default_style = .string
         : pop {
            regex \= (\')
            styles[] = .string;
         }
      }
      : pattern {
         regex \= (\{|\}|\(|\)|\![^\=])
         styles[] = .primary_operators;
      }
      : pattern {
         regex \= (\!\=|\=|\+|\&\&|if)
         styles[] = .secondary_operators;
      }
      : pattern {
         regex \= (true|false|null|\d+)
         styles[] = .keyword;
      }
      : pattern {
         regex \= (?=\b\s?)(\w+)(?=\(.+\)|\!)
         styles[] = .function;
      }
      : pattern {
         regex \= ([a-zA-Z]+)(?=\s+[A-Z]\=)
         styles[] = .parameter;
      }
      # : pattern {
      # regex \= ([a-zA-Z]+)(?=([^a-zA-Z]*|\s+[a-zA-Z]+))(?!\(.+\))(?=([^\!]*)\=)
      #    styles[] = .variable;
      # }
   }
}

styles [] {
   .function : style {
      textmate_scope = "entity.name.function";
      color = "#50fa7b";
   }
   .string : style {
      textmate_scope = "string";
      color = "#f1fa8c";
   }
   .primary_operators : style {
      textmate_scope = "punctuation";
      color = "white";
   }
   .secondary_operators : style {
      textmate_scope = "keyword.operator";
      color = "#ff79c6";
   }
   .keyword : style {
      textmate_scope = "constant";
      color = "#bd93f9";
   }
   .parameter : style {
      textmate_scope = "variable.parameter";
      color = "white";
   }
   # .escaped: style color = "#ff79c6";
   # .equals : style color = "#ffccdd";
   # .id : style color = "#ffb86c";
}







----------------









name = kuma
file_extensions [] = kma;
# background_color = #111116
background_color = #282a36
color = #ffccdd

contexts[] {
   main : context {
      # : pattern {
      #    regex \= (?:\()([a-zA-Z]+)(?:\))
      #   styles[] = .id;
      #}
      : inline_push {
         regex \= (\')
         styles[] = .string;
         default_style = .string
         : pop {
            regex \= (\')
            styles[] = .string;
         }
      }
      : pattern {
         regex \= (\{|\}|\(|\)|\![^\=])
         styles[] = .primary_operators;
      }
      : pattern {
         regex \= (\!\=|\=|\+|\&\&|if)
         styles[] = .secondary_operators;
      }
      : pattern {
         regex \= (true|false|null|\d+)
         styles[] = .keyword;
      }
      : pattern {
         regex \= (?=\b\s?)(\w+)(?=\(.+\)|\!)
         styles[] = .function;
      }
      : pattern {
         regex \= ([a-zA-Z]+)(?=([^a-zA-Z]*|\s+[a-zA-Z]+))(?!\(.+\))(?=([^\!]*)\=)
         styles[] = .variable;
      }
   }
}

styles [] {
   .function : style {
      textmate_scope = "entity.name.function";
      color = "#50fa7b";
   }
   .string : style {
      textmate_scope = "string";
      color = "#f1fa8c";
   }
   .primary_operators : style {
      textmate_scope = "punctuation";
      color = "white";
   }
   .secondary_operators : style {
      textmate_scope = "keyword.operator";
      color = "#ff79c6";
   }
   .keyword : style {
      textmate_scope = "keyword";
      color = "#bd93f9";
   }
   .variable : style {
      textmate_scope = "variable";
      color = "white";
   }
   # .escaped: style color = "#ff79c6";
   # .equals : style color = "#ffccdd";
   # .id : style color = "#ffb86c";
}





----------------------------------------

name = kuma
file_extensions [] = kma;
# background_color = #111116
background_color = #282a36
color = #ffccdd

contexts[] {
   main : context {
      # : pattern {
      #    regex \= (?:\()(\w+)(?:\))
      #   styles[] = .id;
      #}
      : inline_push {
         regex \= (\')
         styles[] = .string;
         default_style = .string
         : pop {
            regex \= (\')
            styles[] = .string;
         }
      }
      : pattern {
         regex \= (\{|\}|\(|\)|\![^\=])
         styles[] = .primary_operators;
      }
      : pattern {
         regex \= (?:\()([a-zA-Z]+)(?:\))
         styles[] = .id;
      }
      : pattern {
         regex \= (\!\=|\=|\+|\&\&|if)
         styles[] = .secondary_operators;
      }
      : pattern {
         regex \= (true|false|null|\d+)
         styles[] = .keyword;
      }
      : pattern {
         regex \= (?=\b\s?)(\w+)(?=\(.+\)|\!)
         styles[] = .function;
      }
      : pattern {
         regex \= ([a-zA-Z]+)(?=([^a-zA-Z]*|\s+[a-zA-Z]+))(?!\(.+\))(?=([^\!]*)\=)
         styles[] = .variable;
      }
   }
}

styles [] {
   .function : style color = "#50fa7b";
   .string : style color = "#f1fa8c";
   .primary_operators : style color = "white";
   .secondary_operators : style color = "#ff79c6";
   .keyword : style color = "#bd93f9";
   .variable : style color = "white";
   # .escaped: style color = "#ff79c6";
   # .equals : style color = "#ffccdd";
   .id : style color = "#ffb86c";
}



------------------


name = kuma
file_extensions [] = kma;
# background_color = #111116
background_color = #282a36
color = #ffccdd

contexts[] {
   main : context {
      # : pattern {
      #    regex \= (?:\()(\w+)(?:\))
      #   styles[] = .id;
      #}
      : inline_push {
         regex \= (\')
         styles[] = .string;
         default_style = .string
         : pop {
            regex \= (\')
            styles[] = .string;
         }
      }
      : pattern {
         regex \= (\{|\}|\(|\)|\!)
         styles[] = .primary_operators;
      }
      : pattern {
         regex \= (\!\=|\=|\+|\&\&|if)
         styles[] = .secondary_operators;
      }
      : pattern {
         regex \= (true|false|null|\d+)
         styles[] = .keyword;
      }
      : pattern {
         regex \= (?=\b\s?)(\w+)(?=\(.+\)|\!)
         styles[] = .function;
      }
      : pattern {
         regex \= ([a-zA-Z]+)(?=([^a-zA-Z]*|\s+[a-zA-Z]+))(?!\(.+\))(?=([^\!]*)\=)
         styles[] = .variable;
      }
   }
}

styles [] {
   .function : style color = "#50fa7b";
   .string : style color = "#f1fa8c";
   .primary_operators : style color = "white";
   .secondary_operators : style color = "#ff79c6";
   .keyword : style color = "#bd93f9";
   .variable : style color = "white";
   # .equals : style color = "#ffccdd";
   # .id : style color = "#ffb86c";
}


------------


name = kuma
file_extensions [] = kma;
# background_color = #111116
background_color = #282a36
color = #ffccdd

contexts[] {
   main : context {
      # : pattern {
      #    regex \= (?:\()(\w+)(?:\))
      #   styles[] = .id;
      #}
      : inline_push {
         regex \= (\')
         styles[] = .string;
         default_style = .string
         : pop {
            regex \= (\')
            styles[] = .string;
         }
      }
      : pattern {
         regex \= (\{|\}|\(|\))
         styles[] = .primary_operators;
      }
      : pattern {
         regex \= (\!\=|\=|\+|\&\&|if)
         styles[] = .secondary_operators;
      }
      : pattern {
         regex \= (true|false|null|\d+)
         styles[] = .keyword;
      }
      : pattern {
         regex \= ([a-zA-Z]+)(?=([^a-zA-Z]*|\s+[a-zA-Z]+))(?!\(.+\))(?=([^\!]+)\=)
         styles[] = .variable;
      }
      : pattern {
         regex \= (?=\b\s?)(\w+)(?=\(.+\)|\!)
         styles[] = .function;
      }
   }
}

styles [] {
   .function : style color = "#50fa7b";
   .string : style color = "#f1fa8c";
   .primary_operators : style color = "white";
   .secondary_operators : style color = "#ff79c6";
   .keyword : style color = "#bd93f9";
   .variable : style color = "white";
   # .equals : style color = "#ffccdd";
   .id : style color = "#ffb86c";
}


----------------


name = kuma
file_extensions [] = kma;
# background_color = #111116
background_color = #282a36
color = #ffccdd

contexts[] {
   main : context {
      : pattern {
         regex \= (\{|\}|\(|\))
         styles[] = .primary_operators;
      }
      : pattern {
         regex \= (\!\=|\=|\+|\&\&|if)
         styles[] = .secondary_operators;
      }
      : pattern {
         regex \= (true|false|null)
         styles[] = .keyword;
      }
      : pattern {
         regex \= (?=\b\s?)(\w+)(?=\(.+\)|\!)
         styles[] = .function;
      }
      : pattern {
         regex \= \b(\w+)\b
         styles[] = .id;
      }
      : pattern {
         regex \= (\w+)(?=>.+)(\=)
         styles[] = .variable, .equals;
      }
      : inline_push {
         regex \= (\')
         styles[] = .string;
         default_style = .string
         : pop {
            regex \= (\')
            styles[] = .string;
         }
      }
   }
}

styles [] {
   .function : style color = "#50fa7b";
   .string : style color = "#f1fa8c";
   .primary_operators : style color = "white";
   .secondary_operators : style color = "#ff79c6";
   .keyword : style color = "#bd93f9";
   .variable : style color = "white";
   .equals : style color = "#ffccdd";
   .id : style color = "#ffb86c";
}









------------------------------
name = kuma
file_extensions [] = kma;
background_color = #111116
color = #ffccdd

contexts[] {
   main : context {
      : pattern {
         regex \= (\{|\}|\(|\))
         styles[] = .primary_operators;
      }
      : pattern {
         regex \= (\!\=|\=)
         styles[] = .secondary_operators;
      }
      : pattern {
         regex         \= (if|true|false|null)
         styles[]       = .keyword;
      }
      : pattern {
         regex \= \b(.+)(\(.+\)|\!)
         styles[] = .function;
      }
      : inline_push {
         regex \= (\')
         styles[] = .string;
         default_style = .string
         : pop {
            regex \= (\')
            styles[] = .string;
         }
      }
   }
}

styles [] {
   .function : style color = "#50fa7b";
   .string : style color = "#f1fa8c";
   .primary_operators : style color = "white";
   .secondary_operators : style color = "#ff79c6";
   .keyword : style color = "#bd93f9";
}
*/


function makeName(firstName, lastName, middleName = null) {
  return
    firstName + ' ' +
    (middleName != null && middleName != '' ? middleName + ' ' : '') +
    lastName;
}

function getFullName(askForMiddleName) {
  let firstName = input('First name?: ');
  let test = firstName;

  let middleName = null;
  if (askForMiddleName)
    middleName = input('Middle name?: ');

  let lastName = input('Last name?: ');

  makeName(firstName, lastName, middleName);
}

println('with middle name: hey, ' + getFullName(true) + '!\n');
println('now without middle name: hey, ' + getFullName(false) + '!');
