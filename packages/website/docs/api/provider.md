---
title: <Provider />
id: provider
slug: /provider
---

## Basic usage
The provider used by components/hooks like [`<Field />`](/docs/field) or [`Form.useFormContext`](/docs/reform).

```rescript {9-11}
module Form = ReForm.Make(Lenses)

@react.component
let make = () => {
  let form = Form.use(
   // ...
  )

  <Form.Provider value=form>
    // Render stuff..
  </Form.Provider>
}
```

## Props
#### `~value`
- Type: [`Form.api`](/docs/reform#api)
- Required: True

#### `children`
- Type: `React.element`
- Required: True
