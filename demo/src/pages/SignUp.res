open Ancestor

module Lenses = %lenses(
  type state = {
    email: string,
    password: string,
    confirmPassword: string,
  }
)

module Form = ReForm.Make(Lenses)

let isTheSamePassword = (state: Lenses.state): ReSchema.fieldState =>
  if state.password == state.confirmPassword {
    Valid
  } else {
    Error(`Password must be confirmed`)
  }

let schema = {
  open Form.Validation

  schema([
    email(Email),
    nonEmpty(Password),
    nonEmpty(ConfirmPassword),
    /*
     * Custom validation
     */
    custom(isTheSamePassword, ConfirmPassword),
  ])
}
@react.component
let make = () => {
  let handleSubmit = ({state}: Form.onSubmitAPI) => {
    Js.Console.log2("Submitted => ", state.values)
    None
  }

  let form = Form.use(
    ~onSubmit=handleSubmit,
    ~validationStrategy=OnDemand,
    ~schema,
    ~initialState={email: "", password: "", confirmPassword: ""},
    (),
  )

  <Box
    px=[xs(4)]
    width=[xs(100.0->#pct)]
    height=[xs(100.0->#pct)]
    justifyContent=[xs(#center)]
    alignItems=[xs(#center)]
    display=[xs(#flex)]>
    <Box
      display=[xs(#flex)]
      flexDirection=[xs(#column)]
      gap=[xs(1.6->#rem->#one)]
      width=[xs(100.0->#pct)]
      maxW=[xs(32.4->#rem)]>
      <Typography tag=#h1 m=[xs(0)] fontSize=[xs(3.2->#rem)]>
        {`Sign up`->React.string}
      </Typography>
      <Input
        value={form.values.email}
        onChange={ReForm.Helpers.handleChange(form.handleChange(Email))}
        placeholder="Email"
        error=?{form.getFieldError(Field(Email))}
      />
      <Input
        value={form.values.password}
        onChange={ReForm.Helpers.handleChange(form.handleChange(Password))}
        type_="password"
        placeholder="Password"
        error=?{form.getFieldError(Field(Password))}
      />
      <Input
        value={form.values.confirmPassword}
        onChange={ReForm.Helpers.handleChange(form.handleChange(ConfirmPassword))}
        type_="password"
        placeholder="Confirm password"
        error=?{form.getFieldError(Field(ConfirmPassword))}
      />
      <Button label="Sign In" onClick={_ => form.submit()} />
    </Box>
  </Box>
}
