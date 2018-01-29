# ReForm.re

[![Greenkeeper badge](https://badges.greenkeeper.io/Astrocoders/reform.svg)](https://greenkeeper.io/)
[![Build Status](https://travis-ci.org/Astrocoders/reform.svg?branch=master)](https://travis-ci.org/Astrocoders/reform)

Reasonably making forms sound good again


## Installation

```
yarn add bs-reform
```

Then add it to bsconfig.json

```
"bs-dependencies": [
 "bs-reform"
]
```

## Usage

Checkout `demo/src/app.re` also

```reason
module SignUpFormParams = {
  type state = {
    password: string,
    email: string
  };
  type fields = [ | `password | `email | `confirmPassword];
  /* (fieldName, getter, setter) */
  let lens = [
    (`email, (s) => s.email, (s, email) => { ...s, email }),
    (`password, (s) => s.password, (s, password) => { ...s, password }),
    (`confirmPassword, (s) => s.confirmPassword, (s, confirmPassword) => { ...s, confirmPassword }),
  ];
};

module SignUpForm = ReForm.Create(SignUpFormParams);

let component = ReasonReact.statelessComponent("SignUp");

let make = (~signInMutation, _children) => {
  ...component,
  render: (_) => {
    <SignUpForm
      initialState={password: "", email: ""}
      schema=[
        (`password, Required),
        (`email, Email),
      ]
      onSubmit=((values, ~setError, ~setSubmitting) => whatever(values, ~setError, ~setSubmitting))
    >
      ...(
        (
           /* this is { values, errors, error }
           * the form.error value is used if you need a global error, submitting error for instance
           */
          ~form,
          ~handleChange,
          ~handleSubmit,
          /* This sets the value of form.error */
          ~handleValidation as _,
          /* A helper to get any field error */
          ~getErrorForField
        ) =>
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
              etc
            </FieldsWrapper>
            <RaisedButton text="Sign in" onPress=handleSubmit/>
          </FormWrapper>
      )
    </SignUpForm>
  }
}
```

## Schema

ReForm accepts a validation schema that looks like 
```reason
[
  (fieldName, getter, validator)
]
```
or

```reason
[
  (`email, s => s.email, Email)
]
```

### Available validators

- _Custom(state => option(string))_
```reason
(`password, s => s.password, Custom(values => values.password == "123" ? Some("Really?") : None))
```
- _Required_
```reason
(`fullName, s => s.fullName, Required)
```
- _Email_
```reason
(`email, s => s.email, Email)
```

#### Any doubts?

If you have any doubts don't hesitate to reach out the wonderful https://discord.gg/reasonml
