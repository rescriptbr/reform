# ReForm.re
[![All Contributors](https://img.shields.io/badge/all_contributors-3-orange.svg?style=flat-square)](#contributors)

Reasonably making forms sound good

- [Installation](#installation)
- [Features](#features)
- [What this is and why](#what-this-is-and-why)
- [Quick usage](#usage)

## Docs

Check our Docusaurus https://astrocoders.dev/reform

## Installation

```
yarn add bs-reform reschema
```

Then add it to bsconfig.json

```
"bs-dependencies": [
 "bs-reform",
 "reschema"
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

## Basic usage

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

module FieldString = {
  [@react.component]
  let make = (~field, ~label) => {
    <PostAddForm.Field
      field
      render={({handleChange, error, value, validate}) =>
        <label>
          <span> {React.string(label)} </span>
          <input
            value
            onChange={Helpers.handleChange(handleChange)}
            onBlur={_ => validate()}
          />
          <p> {error->Belt.Option.getWithDefault("")->React.string} </p>
        </label>
      }
    />;
  };
};

[@react.component]
let make = () => {
  let reform =
    PostAddForm.use(
      ~validationStrategy=OnDemand,
      ~schema={
        PostAddForm.Validation.Schema([|
          StringMin(Title, 20),
          StringNonEmpty(Description),
          Custom(
            AcceptTerms,
            values =>
              values.acceptTerms == false
                ? Error("You must accept all the terms") : Valid,
          )
        |]);
      },
      ~onSubmit=
        ({state}) => {
          Js.log2("title", state.values.description);
          Js.log2("description", state.values.description);
          Js.log2("acceptTerms", state.values.description);
          None;
        },
      ~initialState={title: "", description: "", acceptTerms: false},
      (),
    );

  <PostAddForm.Provider value=reform>
    <form
      onSubmit={event => {
        ReactEvent.Synthetic.preventDefault(event);
        reform.submit();
      }}>
      <FieldString field=StateLenses.Title label="Title" />
      <FieldString field=StateLenses.Description label="Description" />
      <PostAddForm.Field
        field=StateLenses.AcceptTerms
        render={({handleChange, error, value}) =>
          <label>
            <p>
              <span> {"Accept terms? " |> React.string} </span>
              <input
                type_="checkbox"
                value={string_of_bool(value)}
                onChange={event =>
                  ReactEvent.Form.target(event)##checked |> handleChange
                }
              />
            </p>
            <p> {error->Belt.Option.getWithDefault("")->React.string} </p>
          </label>
        }
      />
      {reform.state.formState == Submitting
         ? <p> {React.string("Saving...")} </p>
         : <button type_="submit"> {"Submit" |> React.string} </button>}
    </form>
  </PostAddForm.Provider>;
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

## Contributors ✨

Thanks goes to these wonderful people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore -->
<table>
  <tr>
    <td align="center"><a href="http://www.thomasdeconinck.fr"><img src="https://avatars2.githubusercontent.com/u/1548421?v=4" width="100px;" alt="Thomas Deconinck"/><br /><sub><b>Thomas Deconinck</b></sub></a><br /><a href="https://github.com/Astrocoders/reform/commits?author=DCKT" title="Documentation">📖</a></td>
    <td align="center"><a href="http://twitter.com/fakenickels"><img src="https://avatars0.githubusercontent.com/u/1283200?v=4" width="100px;" alt="Gabriel Rubens"/><br /><sub><b>Gabriel Rubens</b></sub></a><br /><a href="https://github.com/Astrocoders/reform/commits?author=fakenickels" title="Code">💻</a> <a href="https://github.com/Astrocoders/reform/issues?q=author%3Afakenickels" title="Bug reports">🐛</a> <a href="#ideas-fakenickels" title="Ideas, Planning, & Feedback">🤔</a> <a href="#content-fakenickels" title="Content">🖋</a> <a href="https://github.com/Astrocoders/reform/commits?author=fakenickels" title="Documentation">📖</a></td>
    <td align="center"><a href="https://twitter.com/lucasbesen"><img src="https://avatars3.githubusercontent.com/u/13984388?v=4" width="100px;" alt="Lucas Besen"/><br /><sub><b>Lucas Besen</b></sub></a><br /><a href="#maintenance-lucasbesen" title="Maintenance">🚧</a></td>
  </tr>
</table>

<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!
