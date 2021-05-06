---
title: Getting Started
id: getting-started
slug: /getting-started
---

## Quick start

The following code excerpt demonstrates a quick start example:

```rescript
// UserForm.res

module FormFields = %lenses(
  type state = {
    name: string,
    email: string,
  }
)

module UserForm = ReForm.Make(FormFields)

@react.component
let make = (~email) => {
  let form: UserForm.api = UserForm.use(
    ~validationStrategy=OnChange,
    ~onSubmit={(state) => {
      Js.log(state.values)

      None
    }},
    ~initialState={
      name: "",
      // Initialize from external values if you want
      email,
    },
    ~schema={
      open UserForm.Validation
      Schema(nonEmpty(Name) + email(Email))
    },
    (),
  )

  <form
    onSubmit={event => {
      ReactEvent.Synthetic.preventDefault(event)
      form.submit()
    }}>
    <div>
      <label> {React.string("Name")} </label>
      <input
        className="input"
        value={form.values.name}
        onChange={event => onChange((event->ReactEvent.Form.target)["value"])}
        type_="text"
      />
    </div>
    <div>
      <label> {React.string("E-mail")} </label>
      <input
        className="input"
        value={form.values.email}
        onChange={evt => onChange((event->ReactEvent.Form.target)["value"])}
        type_="email"
      />
    </div>
    <button className="button" type_="submit" disabled={form.formState === Submitting}>
      {React.string("Submit form")}
    </button>
  </form>
}
```
