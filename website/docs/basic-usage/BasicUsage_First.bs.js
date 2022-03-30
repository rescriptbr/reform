

import * as Curry from "rescript/lib/es6/curry.js";
import * as React from "react";
import * as ReForm from "@rescriptbr/reform/src/ReForm.bs.js";
import * as Ancestor from "@rescriptbr/ancestor/src/Ancestor.bs.js";
import * as Input$Docs from "../__components/Input.bs.js";
import * as Button$Docs from "../__components/Button.bs.js";

function get(values, field) {
  switch (field) {
    case /* Name */0 :
        return values.name;
    case /* Email */1 :
        return values.email;
    case /* Password */2 :
        return values.password;
    
  }
}

function set(values, field, value) {
  switch (field) {
    case /* Name */0 :
        return {
                name: value,
                email: values.email,
                password: values.password
              };
    case /* Email */1 :
        return {
                name: values.name,
                email: value,
                password: values.password
              };
    case /* Password */2 :
        return {
                name: values.name,
                email: values.email,
                password: value
              };
    
  }
}

var FormFields = {
  get: get,
  set: set
};

var Form = ReForm.Make({
      set: set,
      get: get
    });

function BasicUsage_First(Props) {
  var handleSubmit = function (param) {
    console.log(param.state.values);
    
  };
  var form = Curry._7(Form.use, {
        name: "",
        email: "",
        password: ""
      }, /* Schema */{
        _0: Curry._2(Form.ReSchema.Validation.$plus, Curry._2(Form.ReSchema.Validation.$plus, Curry._6(Form.ReSchema.Validation.string, 3, undefined, undefined, undefined, undefined, /* Name */0), Curry._6(Form.ReSchema.Validation.string, 8, undefined, undefined, undefined, undefined, /* Password */2)), Curry._3(Form.ReSchema.Validation.email, "Invalid email", undefined, /* Email */1))
      }, handleSubmit, undefined, undefined, /* OnDemand */1, undefined);
  var message = Curry._1(form.getFieldError, /* Field */{
        _0: /* Name */0
      });
  var message$1 = Curry._1(form.getFieldError, /* Field */{
        _0: /* Email */1
      });
  var message$2 = Curry._1(form.getFieldError, /* Field */{
        _0: /* Password */2
      });
  return React.createElement(Ancestor.Default.Box.make, {
              display: [Ancestor.Default.xs("flex")],
              flexDirection: [Ancestor.Default.xs("column")],
              alignItems: [Ancestor.Default.xs("center")],
              children: React.createElement(Ancestor.Default.Box.make, {
                    width: [Ancestor.Default.xs({
                            NAME: "pct",
                            VAL: 100.0
                          })],
                    maxW: [Ancestor.Default.xs({
                            NAME: "px",
                            VAL: 320
                          })],
                    children: null
                  }, React.createElement(Ancestor.Default.Base.make, {
                        mb: [Ancestor.Default.xs(1)],
                        fontWeight: [Ancestor.Default.xs(700)],
                        fontSize: [Ancestor.Default.xs({
                                NAME: "px",
                                VAL: 24
                              })],
                        tag: "h1",
                        children: "Sign up"
                      }), React.createElement(Ancestor.Default.Box.make, {
                        children: null
                      }, React.createElement(Input$Docs.make, {
                            value: form.values.name,
                            onChange: (function (e) {
                                return Curry._2(form.handleChange, /* Name */0, e.target.value);
                              }),
                            placeholder: "Your name"
                          }), message !== undefined ? React.createElement(Input$Docs.$$Error.make, {
                              children: message
                            }) : null), React.createElement(Ancestor.Default.Box.make, {
                        mt: [Ancestor.Default.xs(1)],
                        children: null
                      }, React.createElement(Input$Docs.make, {
                            value: form.values.email,
                            onChange: (function (e) {
                                return Curry._2(form.handleChange, /* Email */1, e.target.value);
                              }),
                            placeholder: "Your email"
                          }), message$1 !== undefined ? React.createElement(Input$Docs.$$Error.make, {
                              children: message$1
                            }) : null), React.createElement(Ancestor.Default.Box.make, {
                        mt: [Ancestor.Default.xs(1)],
                        children: null
                      }, React.createElement(Input$Docs.make, {
                            value: form.values.password,
                            onChange: (function (e) {
                                return Curry._2(form.handleChange, /* Password */2, e.target.value);
                              }),
                            type_: "password",
                            placeholder: "Password"
                          }), message$2 !== undefined ? React.createElement(Input$Docs.$$Error.make, {
                              children: message$2
                            }) : null), React.createElement(Ancestor.Default.Box.make, {
                        mt: [Ancestor.Default.xs(1)],
                        children: React.createElement(Button$Docs.make, {
                              children: "Submit",
                              onClick: (function (param) {
                                  return Curry._1(form.submit, undefined);
                                })
                            })
                      }))
            });
}

var make = BasicUsage_First;

export {
  FormFields ,
  Form ,
  make ,
  
}
/* Form Not a pure module */
