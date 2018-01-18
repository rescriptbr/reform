# ReForm.re

[![Greenkeeper badge](https://badges.greenkeeper.io/Astrocoders/reform.svg)](https://greenkeeper.io/)
[![Build Status](https://travis-ci.org/Astrocoders/reform.svg?branch=master)](https://travis-ci.org/Astrocoders/reform)

Reasonably making forms sound good again

## Usage

Checkout `demo/src/app.re` also

```reason
module SignInFormParams = {
  type state = {
    password: string,
    email: string
  };
  type fields = [ | `password | `email ];
  let handleChange = (action, state) =>
    switch action {
    | (`password, value) => {...state, password: value}
    | (`email, value) => {...state, email: value}
    };
};

module SignInForm = ReForm.Create(SignInFormParams);

let component = ReasonReact.statelessComponent("SignIn");

let make = (~signInMutation, _children) => {
  ...component,
  render: (_) => {
    <SignInForm
      initialState={password: "", email: ""}
      schema=[
        (`password, state => state.password, Required),
        (`email, state => state.email, Required),
      ]
      onSubmit=((values, ~setError, ~setSubmitting) => whatever(values, ~setError, ~setSubmitting))
    >
      ...(
        (~form, ~handleChange, ~handleSubmit, ~handleValidation as _, ~getErrorForField) =>
          <FormWrapper>
            <ErrorWarn error=form.error/>
            <FieldsWrapper>
              <FormField
                fieldType=FormField.TextField
                value=form.values.email
                placeholder="Email"
                style=fieldsStyle
                placeholderTextColor=AppTheme.Colors.blackLight
                /* handleChange signature is (fields, string) => unit, so you can use right away with RN and React Web.
                ** Just make an abstraction above to not give it an event and just the value
                */
                onChangeText=handleChange(`email)
              />
              /* getErrorForField returns a option(string) */
              <ErrorText value=getErrorForField(`password)/>
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
    </SignInForm>
  }
}
```
