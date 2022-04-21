module FormFields = %lenses(
  type state = {
    name: string,
    email: string,
    password: string,
  }
)

module Form = ReForm.Make(FormFields)

@react.component
let make = () => {
  // highlight-start
  let handleSubmit = ({state}: Form.onSubmitAPI) => {
    Js.log(state.values)

    None
  }

  let _form = Form.use(
    ~initialState={name: "", email: "", password: ""},
    ~onSubmit=handleSubmit,
    ~validationStrategy=OnDemand,
    ~schema={
      open Form.Validation

      schema([string(~min=3, Name), string(~min=8, Password), email(~error="Invalid email", Email)])
    },
    (),
  )

  // highlight-end

  React.null
}
