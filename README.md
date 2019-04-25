# ReForm.re

[![Greenkeeper badge](https://badges.greenkeeper.io/Astrocoders/reform.svg)](https://greenkeeper.io/)
[![Build Status](https://travis-ci.org/Astrocoders/reform.svg?branch=master)](https://travis-ci.org/Astrocoders/reform)

Reasonably making forms sound good again (pun 100% intended)

* [Installation](#installation)
* [What this is and why](#what-this-is-and-why)
* [Quick usage](#usage)

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
Code that deals with strongly typed forms can quickly become walls of repeated text.
We created ReForm to be both deadly simple and to make forms sound good leveraging ReasonML's powerful typesytem.
Even the schemas we use are nothing more than constructors built-in in the language itself with a small size footprint.

## Usage with hooks

Checkout `packages/example/src/PostAddNext.re` also

```reason
[%lenses
  type state = {
    description: string,
    title: string,
    acceptTerms: bool,
  }
];
module PostAddForm = ReFormNext.Make(StateLenses);

[@react.component]
let make = _children => {
  let {state, submit, getFieldState, handleChange}: PostAddForm.api =
    PostAddForm.use(
      ~schema={
        PostAddForm.Validation.Schema([|
          Custom(
            Title,
            values =>
              Js.String.length(values.title) > 20
                ? Error("Keep it short!") : Valid,
          ),
          Custom(
            Description,
            values =>
              values.description == "" ? Error("Can't be empty") : Valid,
          ),
          Custom(
            AcceptTerms,
            values =>
              values.acceptTerms == false
                ? Error("You must accept all the terms") : Valid,
          ),
        |]);
      },
      ~onSubmit=
        ({state}) => {
          mutate(
            ~variables=
              PostAddMutationConfig.make(
                ~title=state.values.title,
                ~description=state.values.description,
                (),
              )##variables,
            (),
          )
          |> Js.Promise.then_(result =>
               setResult(_ => Some(result)) |> Js.Promise.resolve
             )
          |> ignore;

          None;
        },
      ~initialState={title: "", description: "", acceptTerms: false},
      (),
    );
};
```

#### Support

The authors regularly hang out at the wonderful https://discord.gg/reasonml or https://reasonml.chat so feel free to visit us there.
