module Form = SignUp_Form
module Styles = SignUp_Styles

let onSubmit = result => {
  Js.log(result)

  None
}

@react.component
let make = () => {
  let form: Form.api = Form.use(
    ~validationStrategy=OnDemand,
    ~onSubmit,
    ~initialState=Form.initialState,
    ~schema={
      open Form.Validation
      Schema(nonEmpty(Name) + email(Email))
    },
    (),
  )

  Js.log(form.getFieldError(Form.ReSchema.Field(Form.Fields.Name)))

  <form
    className={Styles.form}
    onSubmit={e => {
      FormHelpers.preventDefault(e)
      form.validateForm()
      form.submit()
    }}>
    <Box margin=(0, 0, 1, 0)>
      <Input
        type_="text"
        onChange=form.handleChange
        field=Form.Fields.Name
        label="Name"
        error={form.getFieldError(Form.ReSchema.Field(Form.Fields.Name))}
      />
    </Box>
    <Box margin=(0, 0, 1, 0)>
      <Input
        type_="email"
        onChange=form.handleChange
        field=Form.Fields.Email
        label="E-mail"
        error={form.getFieldError(Form.ReSchema.Field(Form.Fields.Email))}
      />
    </Box>
    <Box block=true> <Button type_="submit"> "SIGN UP" </Button> </Box>
  </form>
}
