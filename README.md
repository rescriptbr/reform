# ReForm.re

[![Greenkeeper badge](https://badges.greenkeeper.io/Astrocoders/reform.svg)](https://greenkeeper.io/)
[![Build Status](https://travis-ci.org/Astrocoders/reform.svg?branch=master)](https://travis-ci.org/Astrocoders/reform)

![ReForm demo](./website/static/img/fulldemo.png)

Reasonably making forms sound good again (pun 100% intended)

* [Installation](#installation)
* [What this is and why](#what-this-is-and-why)
* [Quick usage](#usage)
* [API](#api)
  * [Component params](#component-params)
  * [onSubmit](#onsubmit)
  * [Schema](#schema)
  * [Available validators](#available-validators)
  * [reform.form](#form)
  * [reform.getErrorForField](#geterrorforfield)
  * [reform.handleSubmit](#handlesubmit)
  * [reform.handleChange](#handlechange)
  * [reform.handleGlobalValidation](#handleglobalvalidation)

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

## What this is and why
Dealing with forms directly can scalate really quickly to a hell when not using the right approach.
We created ReForm to be both deadly simple and to make forms sound good leveraging ReasonML's powerful typesytem.
Even the schemas we use are nothing more than constructors built-in in the language itself with a small size footprint.

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
        ({
          form,
          handleChange,
          handleSubmit,
          getErrorForField
        }) =>
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

# API
We tried to make the API simple yet very powerful, so you don't have to worry about learning a lot of quirks

## Component params
When you create a new ReForm module you get a brand new ReasonReact component
```reason
/* Just some regular ReasonReact guy */
module Form = ReForm.Create(SignUpFormParams);
```
These are the props/params it accepts:
### <Form schema>
The schema tells to ReForm how to validate your date, take a look at [Schema](#schema) to see more

### <Form validate>, validate: Config.values => option(string)
We let this scape hatch for when the provided validators aren't enough for you and you need some more complexity.
```reason
let validate: SignUpForm.values => option(string) = (values) => {
  switch (values) {
    | { email: "unsafeTypeGuy@ohno.com" } when values.password === "secretThing" => Some("Can't do.")
    | _ => None
  }
}

<Form
  validate
  /* Yes! You can still use schema with it */
  schema=[(`email, Email)]
>
```

The returned valued of `validate` will set `reform.form.error`

### <Form onSubmit>, onSubmit(values, ~setError, ~setSubmitting) => unit
This is the guy you'll be putting your POST/mutation/whatever logic into, it is triggered after `handleSubmit` is called.

```reason
let onSubmit = (values, ~setError, ~setSubmitting) => {
  Js.Promise.(
    values 
    |> convertToJS
    |> mutate
    |> then_(response => {
        switch(response##error |> Js.Null_undefined.to_opt) {
          | None =>
            setSubmitting(false);
            doSomeOtherThing();
          | Some(error) =>
            setSubmitting(false);
            setError(Some("Something went wrong, try again"))
        }
      })
  )
  |> ignore
}
```

### <Form i18n>, i18n: ReForm.Validation.I18n.dictionary
You can pass a custom dictionary to be shown as the validators errors

## children: (YourForm.reform => ReasonReact.reactElement)
The param passed to the children is a record of the following type
```reason
type reform = {
  form: state,
  handleChange: (Config.fields, value) => unit,
  handleGlobalValidation: option(string) => unit,
  handleSubmit: unit => unit,
  getErrorForField: Config.fields => option(string)
};
```
### form: Params.state
Accessed via `reform.form` and contains the following
```reason
{
  /* The record containing the actual form state */
  values: Params.state,
  /* The submitting state */
  isSubmitting: bool,
  /* This is intended to store global validation error, like a submitting failure */
  error: option(string)
}
```

### handleChange: (Config.fields, string) => unit
`handleChange` takes the field in question and its string value, we made like this so you can use it both in Web and React Native

### handleSubmit: unit => unit
Triggers the submitting and makes ReForm set `reform.form.isSubmitting` to true

### getErrorForField: Config.fields => options(string)
Returns the validation error, if there is any, for the field in question

### handleGlobalValidation: option(string) => unit
Handles the global error value at `reform.form.error`

## Schema

The schema used by ReForm is nothing more than a tuple and the validator is a [constructor](http://2ality.com/2017/12/variants-reasonml.html#variants-as-data-structures) thus the final representation is really lightweight and does not enforce you to bring Yet Another Schema Validator For JS.

It is passed as a param to the form, `<SignInForm schema>`

ReForm accepts a validation schema that looks like 
```reason
(fieldName, getter, validator)
```
or

```reason
(`email, s => s.email, Email)
```

Take a look in the demo to see it in action.

### Available validators

#### Custom(state => option(string))
```reason
(`password, s => s.password, Custom(values => values.password == "123" ? Some("Really?") : None))
```
#### Required
```reason
(`fullName, s => s.fullName, Required)
```
#### Email
```reason
(`email, s => s.email, Email)
```
