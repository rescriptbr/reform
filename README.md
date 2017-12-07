# ReForm.re

[![Greenkeeper badge](https://badges.greenkeeper.io/Astrocoders/bs-package-boilerplate.svg)](https://greenkeeper.io/)
[![Build Status](https://travis-ci.org/Astrocoders/bs-package-boilerplate.svg?branch=master)](https://travis-ci.org/Astrocoders/bs-package-boilerplate)

Reasonably making forms sound good again

## Usage

```reason
module SignUpFormParams = {
  type state = {
    password: string,
    email: string,
    confirmPassword,
  };
  let initialState = {password: "", email: "", confirmPassword: ""};
  type fields = [ | `password | `email ];
  let handleChange = (action, state) =>
    switch action {
    | (`password, value) => {...state, password: value}
    | (`confirmPassword, value) => {...state, password: value}
    | (`email, value) => {...state, email: value}
    };
};

module SignUpForm = ReForm.Create(SignUpFormParams);

let component = ReasonReact.statelessComponent("SignUpForm");

let make = (~signUpMutation, _children) => {
  ...component,
  render: (_) => {
    /* Global validation if you want */
    let validate: SignUpFormParams.state => option(string) = (values) => switch values {
      | { password: "12345" } => Some("Sorry, can't do")
      | _ => None
    }

    <ReForm
      onSubmit=((values, ~setError, ~setSubmitting) => whatever(values, ~setError, ~setSubmitting))
      schema=[
        (`password, [Required]),
        (`confirmPassword, [Custom(({ confirmPassword, password }) => confirmPassword != password ? Some("Password don't match") : None)]),
        (`email, [Required, Email])
      ]
      validate
    >
      (
        (~form, ~handleChange, ~handleSubmit) =>
          <FormWrapper>
            <ErrorWarn error=form.error/>
            <FieldsWrapper>
              <FormField
                fieldType=FormField.TextField
                value=form.values.email
                placeholder="Email"
                style=fieldsStyle
                placeholderTextColor=AppTheme.Colors.blackLight
                onChangeText=handleChange(`email)
              />
              <FormField
                fieldType=FormField.TextField
                placeholder="Password"
                onChangeText=handleChange(`password)
                value=form.values.password
                style=fieldsStyle
                placeholderTextColor=AppTheme.Colors.blackLight
              />
            </FieldsWrapper>
            <RaisedButton text="Sign in" onPress=handleSubmit/>
            </FormWrapper>
      )
    </ReForm>
  }
}
```
