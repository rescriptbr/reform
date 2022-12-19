---
title: <Field />
id: field
slug: /field
---

ReForm provides a `Field` component that is optimized using `React.useMemo`. It prevents unnecessary re-renders that happens when you're using only the `Form.use` hook. 
:::important
`<Field />` depends on [`<Form.Provider />`](./provider). 
:::
### Basic usage
```rescript
module Form = ReForm.Make(Lenses)

@react.component
let make = () => {
  let form = Form.use(
    ~schema,
    ~onSubmit=_ => None,
    ~initialState={name:""},
    ()
  )
  
  /*
  * We need to put the Provider in the top. 
  */
  <Form.Provider value=form>
    <Form.Field 
      field=Name
      render={field => 
        <input 
          value=field.value 
          onChange=ReForm.Helpers.handleChange(field.handleChange) 
        />
      }
    />
  </Form.Provider>
}
```

### Props

#### `render`
- Type: [`fieldInterface<'a> `](/docs/reform#fieldinterface) ` => React.element`
- Required: True

#### `field`
- Type: [`Config.field<'a>`](/docs/reform-make)
- Required: True

#### `renderOnMissingContext`
- Type: `React.element`
- Required: False
- Default value: `React.null`
