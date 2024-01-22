## API Reference
- [`Make(...)`](#reschemamake)
- [`type fieldState`](#fieldstate)
- [`type recordValidationState<'a>`](#recordvalidationstatea)
- [`let validate`](#validate)
- [`let validateField`](#validatefield)
- [`let validateFields`](#validatefields)
- [`let validateOne`](#validatefields)
- [Validation](#module-validation)
  - [`type t`](#type-t)
  - [`type schema`](#schema)
  - [`Validators`](#validators)
    - [`custom(...)`](#custom)
    - [`true_(...)`](#true_)
    - [`false(...)`](#false_)
    - [`email(...)`](#email)
    - [`nonEmpty(...)`](#email)
    - [`string(...)`](#string)
    - [`regExp(...)`](#regexp)
    - [`float(...)`](#float)
    - [`int(...)`](#int)

### `ReSchema.Make(...)`
The module functor `ReForm.Make` receives a module type with the following signature:

```rescript
module type Lenses = {
  type field<'a>
  type state
  let set: (state, field<'a>, 'a) => state
  let get: (state, field<'a>) => 'a
}
```
> 💡 This module can be generated using [`lenses-ppx`](https://github.com/rescriptbr/lenses-ppx).

### `fieldState`
The field state.
```rescript
type fieldState =
  | Valid
  | NestedErrors(array<childFieldError>)
  | Error(string)
```

### `recordValidationState<'a>`
```rescript
type recordValidationState<'a> =
  | Valid
  | Errors(array<('a, string)>)
```

### `validate(...)`
```rescript
let validate: (
  ~i18n: ReSchemaI18n.t=?,
  Lenses.state,
  Validation.schema,
) => recordValidationState<field>
```
### `validateField(..)`
```rescript
let validateField: (
  ~validator: Validation.t,
  ~values: Lenses.state,
  ~i18n: ReSchemaI18n.t,
) => (field, fieldState)
```
### `validateFields(..)`
```rescript
let validateFields: (
  ~fields: array<field>,
  ~values: Lenses.state,
  ~i18n: ReSchemaI18n.t,
  Validation.schema,
) => array<option<(field, fieldState)>>
```
### `validateOne(...)`
```rescript
let validateOne: (
  ~field: field,
  ~values: Lenses.state,
  ~i18n: ReSchemaI18n.t=?,
  Validation.schema,
) => option<(field, fieldState)>
```
### `getFieldValidators(...)`
```rescript
let getFieldValidators: (
  ~validators: array<Validation.t>,
  ~fieldName: field,
) => array<Validation.t>
```
### `getFieldValidator(...)`
```rescript
let getFieldValidator: (
  ~validators: array<Validation.t>,
  ~fieldName: field,
) => option<Validation.t>
```

## `module RegExps`

### `email`
```rescript
let email: Js.Re.t
```

## `module Validation`
This is the validation module. It contains the main functions and structures that you're going to use to create and validate schemas.

### `type t`
```rescript
type rec t =
    | Email({
        field: Lenses.field<string>, 
        error: option<string>
      }): t
    | NoValidation({
        field: Lenses.field<'a>
      }): t
    | StringNonEmpty({
        field: Lenses.field<string>, a
        error: option<string>
      }): t
    | StringRegExp({
        field: Lenses.field<string>, 
        matches: string, 
        error: option<string>
      }): t
    | StringMin({
        field: Lenses.field<string>, 
        min: int, 
        error: option<string>
      }): t
    | StringMax({
        field: Lenses.field<string>, 
        max: int, 
        error: option<string>
      }): t
    | IntMin({
        field: Lenses.field<int>, 
        min: int, 
        error: option<string>
      }): t
    | IntMax({
        field: Lenses.field<int>, 
        max: int, 
        error: option<string>
      }): t
    | FloatMin({
        field: Lenses.field<float>, 
        min: float, 
        error: option<string>
      }): t
    | FloatMax({
        field: Lenses.field<float>, 
        max: float, 
        error: option<string>
      }): t
    | Custom({
        field: Lenses.field<'a>, 
        predicate: Lenses.state => fieldState
      }): t
    | True({
        field: Lenses.field<bool>, 
        error: option<string>
      }): t
    | False({
        field: Lenses.field<bool>, 
        error: option<string>
      }): t
```

### `schema`
```rescript
type schema = array<t>
```

### Validators
ReSchema provides some validators built on top of the `schema` type.
This functions can be very useful to create validations without adding a lot of variant constructors to the schema array.

#### `custom(...)`
It's used to create custom validations by passing a `predicate` and `field` that will be validated.
```rescript
let custom: (Lenses.state => fieldState, Lenses.field<'a>) => array<schema>
```

#### `true_(...)`
```rescript
let true_: (~error: option<string>, Lenses.field<'a>) => array<schema>
```
#### `false_(...)`
```rescript
let false_: (~error: option<string>, Lenses.field<'a>) => array<schema>
```
#### `email(...)`
```rescript
let email: (~error: option<string>, Lenses.field<'a>) => array<schema>
```
#### `nonEmpty(...)`
```rescript
let nonEmpty: (~error: option<string>, Lenses.field<'a>) => array<schema>
```
#### `string(...)`
```rescript
let string: (
  ~min: option<int>, 
  ~minError: option<string>, 
  ~max: option<int>, 
  ~maxError: option<string>,
  Lenses.field<'a>
) => array<schema>
```
#### `regExp(...)`
```rescript
let regExp: (~error: option<string>, ~matches: Js.RegExp.t, Lenses.field<'a>) => array<schema>
```
#### `float(...)`
```rescript
let float: (
  ~min: option<float>, 
  ~minError: option<string>, 
  ~max: option<float>, 
  ~maxError: option<string>,
  Lenses.field<'a>
) => array<schema>
```
#### `int(...)`
```rescript
let int: (
  ~min: option<int>, 
  ~minError: option<string>, 
  ~max: option<int>, 
  ~maxError: option<string>,
  Lenses.field<'a>
) => array<schema>
```

