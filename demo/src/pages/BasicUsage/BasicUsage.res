module FormFields = %lenses(
  type state = {
    name: string,
    email: string,
  }
)

module UserForm = ReForm.Make(FormFields)

let onSubmit = ({state}: UserForm.onSubmitAPI) => {
  Js.log(state.values)

  None
}

let initialState: FormFields.state = {
  name: "",
  email: "",
}

@react.component
let make = () => {
  let form: UserForm.api = UserForm.use(
    ~validationStrategy=OnChange,
    ~onSubmit,
    ~initialState,
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
        onChange={ReForm.Helpers.handleChange(form.handleChange(FormFields.Name))}
        type_="text"
      />
    </div>
    <div>
      <label> {React.string("E-mail")} </label>
      <input
        className="input"
        value={form.values.email}
        onChange={ReForm.Helpers.handleChange(form.handleChange(FormFields.Email))}
        type_="email"
      />
    </div>
    <button className="button" type_="submit" disabled={form.formState === Submitting}>
      {React.string("Submit form")}
    </button>
  </form>
}
