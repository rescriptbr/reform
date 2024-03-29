---
title: ReForm.use(...)
id: reform-use
slug: /reform-use
---

### The `use` form hook

`ReForm.use` is the mainly hook. You're always going to use it to build your forms. It's important to notice that the hook is only available in your form module generated by the [ReForm.Make](/docs/reform-make) functor.

```rescript
module Lenses = %lenses(
  type state = {
   name: string
  }
)

module Form = ReForm.Make(Lenses)

@react.component
let make = () => {
 let form = Form.use(
  ~initialState={ name: "Alonzo Church" },
  ~onSubmit={api => {
    Js.log(api)
    None
  }},
  ~schema={
    open Form.Validation

    schema([string(~min=3, Name)])
    },
  ()
  )

  // Render stuff...
}
```

### Args 

#### `~initalState`         
This is the inital state of your form. Its type signature is the state record that you defined in your lenses module like [here](/docs/getting-started#creating-our-form).
- Type: `Config.state`
- Required: Yes

#### `~onSubmit`
Then function that will be called on form submit.
- Type: [`Form.onSubmitAPI`](/docs/reform#onsubmitapi) ` => option<unit => unit>`
- Required: Yes

#### `~onSubmitFail`
Then function that will be called when the submit fails.
- Type: [`Form.onSubmitAPI`](/docs/reform#onsubmitapi) ` => option<unit => unit>`
- Required: No
- Default value: `ignore`

#### `~i18n`
Your custom i18n module created with ReSchema.
> Check out ReSchema's [official docs](https://github.com/rescriptbr/reschema).

- Type: `ReSchemaI18n.t`
- Required: No
- Default value: `ReSchemaI18n.default`

#### `~validationStrategy`
The validation strategy to validate the form fields.
- Type: [`ReForm.validationStrategy`](/docs/reform#validationstrategy)
- Required: No
- Default value: `OnChange`

### Returns
The return of the hook (which we named as `form` in our example) is a record typed as [`ReForm.api`](/docs/reform#api).

