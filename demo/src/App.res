module FormLenses = %lenses(
  type state = {
    name: string,
    age: int,
  }
)

module UserForm = ReForm.Make(FormLenses)

let str = React.string

let onSubmit = state => {
  Js.log(state)
  None
}

let updateField = (field, fn, event) => fn(field, ReactEvent.Form.target(event)["value"])

@react.component
let make = () => {
  let form: UserForm.api = UserForm.use(
    ~onSubmit,
    ~initialState={name: "", age: 0},
    ~schema={
      open UserForm.Validation
      Schema(string(~min=12, Name))
    },
    (),
  )

  <form
    onSubmit={event => {
      event->ReactEvent.Synthetic.preventDefault
      form.submit()
    }}>
    <input
      type_="text"
      placeholder="Name"
      value=form.values.name
      onChange={updateField(FormLenses.Name, form.handleChange)}
    />
    <button type_="submit"> {str(`Click here`)} </button>
  </form>
}
