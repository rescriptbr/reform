# ReForm.re

[![Greenkeeper badge](https://badges.greenkeeper.io/Astrocoders/bs-package-boilerplate.svg)](https://greenkeeper.io/)
[![Build Status](https://travis-ci.org/Astrocoders/bs-package-boilerplate.svg?branch=master)](https://travis-ci.org/Astrocoders/bs-package-boilerplate)

Reasonably making forms sound good again

## Usage
```ml
module SignInFormParams = {
  type state = {
    password: string,
    email: string
  };
  let initialState = {password: "", email: ""};
  let handleChange action state =>
    switch action {
    | ReForm.HandleChange ("password", value) => {...state, password: value}
    | ReForm.HandleChange ("email", value) => {...state, email: value}
    | _ => state
    };
};

module ReForm = ReForm.Create SignInFormParams;

let component = ReasonReact.statelessComponent "SignInForm";

let make _children => {
  ...component,
  render: fun _ =>
    <ReForm handleSubmit=(fun _ => ignore ())>
      (
        fun form handleChange handleSubmit =>
          <FormWrapper>
            <FieldsWrapper>
              <FormField
                fieldType=FormField.TextField
                value=form.values.email
                placeholder="Email"
                style=fieldsStyle
                placeholderTextColor=AppTheme.Colors.blackLight
                onChangeText=(handleChange "email")
              />
              <FormField
                fieldType=FormField.TextField
                placeholder="Password"
                onChangeText=(handleChange "password")
                value=form.values.password
                style=fieldsStyle
                placeholderTextColor=AppTheme.Colors.blackLight
              />
            </FieldsWrapper>
            <RaisedButton text="Sign in" onPress={handleSubmit}/>
            </FormWrapper>
      )
    </ReForm>
}
```
