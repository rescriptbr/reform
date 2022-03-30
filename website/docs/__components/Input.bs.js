

import * as React from "react";
import * as Ancestor from "@rescriptbr/ancestor/src/Ancestor.bs.js";
import * as Caml_option from "rescript/lib/es6/caml_option.js";
import * as Css from "@emotion/css";

var input = Css.css({
      backgroundColor: "#f6f6f6",
      color: "var(--ifm-heading-font-color)",
      fontFamily: "DM Sans",
      outline: "none",
      border: "none",
      width: "100%",
      borderRadius: 8,
      fontSize: 16,
      padding: 16
    });

function Input(Props) {
  var value = Props.value;
  var onChange = Props.onChange;
  var name = Props.name;
  var type_Opt = Props.type_;
  var placeholder = Props.placeholder;
  var type_ = type_Opt !== undefined ? type_Opt : "text";
  var tmp = {
    tag: "input",
    className: input,
    onChange: onChange,
    type_: type_,
    value: value
  };
  if (name !== undefined) {
    tmp.name = Caml_option.valFromOption(name);
  }
  if (placeholder !== undefined) {
    tmp.placeholder = Caml_option.valFromOption(placeholder);
  }
  return React.createElement(Ancestor.Default.Base.make, tmp);
}

function Input$Error(Props) {
  var children = Props.children;
  return React.createElement(Ancestor.Default.Base.make, {
              color: [Ancestor.Default.xs({
                      NAME: "hex",
                      VAL: "#f36"
                    })],
              m: [Ancestor.Default.xs(0)],
              fontSize: [Ancestor.Default.xs({
                      NAME: "px",
                      VAL: 12
                    })],
              tag: "p",
              children: children
            });
}

var $$Error = {
  make: Input$Error
};

var make = Input;

export {
  input ,
  make ,
  $$Error ,
  
}
/* input Not a pure module */
