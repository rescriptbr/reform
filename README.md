# ReForm.re

[![Greenkeeper badge](https://badges.greenkeeper.io/Astrocoders/reform.svg)](https://greenkeeper.io/)
[![Build Status](https://travis-ci.org/Astrocoders/reform.svg?branch=master)](https://travis-ci.org/Astrocoders/reform)

![ReForm demo](./website/static/img/fulldemo.png)

Reasonably making forms sound good again (pun 100% intended)

* [Installation](#installation)
* [What this is and why](#what-this-is-and-why)
* [Quick usage](#usage)
* [API](#validation)
  * [Schema](#schema)
  * [Available validators](#available-validators)
  * [reform.getErrorForField](#reform.getErrorForField)

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

## Schema

The schema used by ReForm is nothing more than a tuple and the validator is a [constructor](http://2ality.com/2017/12/variants-reasonml.html#variants-as-data-structures) thus the final representation is really lightweight and does not enforce you to bring Yet Another Schema Validator For JS.

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
