

import * as React from "react";
import * as Caml_option from "rescript/lib/es6/caml_option.js";
import * as Css from "@emotion/css";

var button = Css.css({
      cursor: "pointer",
      background: "var(--red)",
      borderRadius: 8,
      width: "100%",
      color: "#fff",
      fontFamily: "DM Sans",
      padding: 16,
      fontWeight: 700,
      border: 0,
      fontSize: 18,
      "&:hover": {
        backgroundColor: "var(--ifm-color-primary-dark)"
      }
    });

var Styles = {
  button: button
};

function Button(Props) {
  var children = Props.children;
  var onClick = Props.onClick;
  var tmp = {
    className: button
  };
  if (onClick !== undefined) {
    tmp.onClick = Caml_option.valFromOption(onClick);
  }
  return React.createElement("button", tmp, children);
}

var make = Button;

export {
  Styles ,
  make ,
  
}
/* button Not a pure module */
