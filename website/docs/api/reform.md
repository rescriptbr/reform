---
title: ReForm
id: reform
slug: /reform
---

## Overview 
This is the api reference for the main module `ReForm`.

## Types

### `fieldState`
This is the state of the field. It tells you when the field is valid, errored, etc.
```rescript
type fieldState =
  | Pristine
  | Valid
  | NestedErrors(array<ReSchema.childFieldError>)
  | Error(string)
```

### `formState`
This is the state of the form. It tells you whe  the form is valid, has errors, dirty, etc.
```rescript
type formState =
  | Dirty
  | Submitting
  | Pristine
  | Errored
  | SubmitFailed(option<string>)
  | Valid
  ```

### `action`
This is the action type. It contains all actions that reform dispatches to update and interact with the form state.
You might need use this action when using the [`onSubmitAPI.send`](/docs/reform#onsubmitapi) function.
```rescript
type rec action =
    | ValidateField(field)
    | ValidateForm(bool)
    | TrySubmit
    | Submit
    | SetFieldsState(array<(field, fieldState)>)
    | FieldChangeState(field, fieldState)
    | FieldChangeValue(Config.field<'a>, 'a): action
    | FieldChangeValueWithCallback(Config.field<'a>, 'a => 'a): action
    | FieldArrayAdd(Config.field<array<'a>>, 'a): action
    | FieldArrayUpdateByIndex(Config.field<array<'a>>, 'a, int): action
    | FieldArrayRemove(Config.field<array<'a>>, int): action
    | FieldArrayRemoveBy(Config.field<array<'a>>, 'a => bool): action
    | SetFormState(formState)
    | ResetForm
    | SetValues(Config.state => Config.state)
    | SetFieldValue(Config.field<'a>, 'a): action
    | RaiseSubmitFailed(option<string>)
```

### `onSubmitAPI`
:::tip
If you need to use the `send` function, check out the [`action`](/docs/reform#action) type signature.
:::
```rescript
type onSubmitAPI = {
  send: action => unit,
  state: state,
  raiseSubmitFailed: option<string> => unit,
}
```

### `state`
This is the state of the form. It contains the values of all form fields, the fields state, etc.
```rescript
type state = {
  formState: formState,
  values: Config.state,
  fieldsState: array<(field, fieldState)>,
}
```

#### `validationStrategy`
This is the available validations strategies.
```rescript
type validationStrategy =
  | OnChange
  | OnDemand
  ```

### `api`
This type is a record with following fields:

#### `state`
- Type: `state`

#### `values`
- Type: `Config.state`

#### `formState`
- Type: `formState`

#### `fieldsState`
- Type: `array<(field, fieldState)>`

#### `isSubmitting`
- Type: `bool`

#### `isDirty`
- Type: `bool`

#### `isPristine`
- Type: `bool`

#### `getFieldState`
- Type: `field => fieldState`

#### `getFieldError`
- Type: `field => option<string>`

#### `getNestedFieldError`
- Type: ``(field, int) => option<string>``

#### `handleChange`
- Type: `'a. (Config.field<'a>, 'a) => unit`

#### `handleChangeWithCallback`
- Type: `'a. (Config.field<'a>, 'a => 'a) => unit`

#### `arrayPush`
- Type: `'a. (Config.field<array<'a>>, 'a) => unit`

#### `arrayUpdateByIndex`
- Type: `'a. (~field: Config.field<array<'a>>, ~index: int, 'a) => unit`

#### `arrayRemoveByIndex`
- Type: `'a. (Config.field<array<'a>>, int) => unit`

#### `arrayRemoveBy`
- Type: `'a. (Config.field<array<'a>>, 'a => bool) => unit`

#### `submit`
- Type: `unit => unit`

#### `resetForm`
- Type: `unit => unit`

#### `setValues`
- Type: ``(Config.state => Config.state) => unit``

#### `setFieldValue`
- Type: `'a. (Config.field<'a>, 'a, ~shouldValidate: bool=?, unit) => unit`

#### `validateField`
- Type: `field => unit`

#### `validateForm`
- Type: `unit => unit`

#### `validateFields`
- Type: `array<field> => array<fieldState>`

#### `raiseSubmitFailed`
- Type: `option<string> => unit`

