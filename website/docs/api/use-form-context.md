---
title: useFormContext(...)
id: use-form-context
slug: /use-form-context
---

## Basic usage
```rescript {6,8-11}
module Form = ReForm.Make(Lenses)

module Field = {
  @react.component
  let make = () => {
    let form = Form.useFormContext()

    {switch form {
     | None => React.null
     | Some(form) =>  <input value=form.values.name />
    }}
  }
}

@react.component
let make = () => {
  let form = Form.use(
   ...
  )

  <Form.Provider value=form>
    <Field />
  </Form.Provider>
}
```

## API
The `useFormContext(...)` returns `option<api>` (an option of [api](/docs/reform#api)).
