# ReForm.re

Reasonably making forms sound good

* [Installation](#installation)
* [Features](#features)
* [What this is and why](#what-this-is-and-why)
* [Quick usage](#usage)

## Docs
Check our Docusaurus https://astrocoders.dev/reform

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

Then add lenses-ppx
```
yarn add lenses-ppx@4.0.0 -D
```

And update your bsconfig.json with `ppx-flags`
```
"ppx-flags": [
 "lenses-ppx/ppx"
]
```

## Features
- Hook API
- Schema API
- Type safe, `handleChange` properly infers the value of the field it is handling
- Context Provider
- Field component
- Validation strategy, OnDemand and OnChange

## What this is and why
Code that deals with strongly typed forms can quickly become walls of repeated text.
We created ReForm to be both deadly simple and to make forms sound good leveraging ReasonML's powerful typesytem.
Even the schemas we use are nothing more than constructors built-in in the language itself with a small size footprint.

## Usage with hooks

Checkout https://github.com/Astrocoders/reform/blob/master/packages/demo/src/PostAddNext.re for a more complete demo

```reason
open BsReform;

module StateLenses = [%lenses
  type state = {
    description: string,
    title: string,
    acceptTerms: bool,
  }
];
module PostAddForm = ReForm.Make(StateLenses);

[@react.component]
let make = () => {
  let {state, submit, getFieldState, handleChange}: PostAddForm.api =
    PostAddForm.use(
      ~schema={
        PostAddForm.Validation.Schema([|
          StringMin(Title, 20),
          StringNonEmpty(Description),
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

#### Alternatives
- The great https://github.com/alexfedoseev/re-formality

#### Publishing

```
lerna version major|patch|minor
```

and then

```
lerna publish from-git
```

#### Support

We usually hang out at https://discord.gg/reasonml or https://reasonml.chat so feel free to ask anything there.
