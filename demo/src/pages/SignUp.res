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
    string(~min=6, Password),
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
      <Button label="Sign Up" onClick={_ => form.submit()} />
      <Typography
        color=[xs(#hex("#000"))]
        href="https://github.com/rescriptbr/reform/blob/master/demo/src/pages/SignUp.res"
        target="_blank"
        textAlign=[xs(#center)]
        fontSize=[xs(1.6->#rem)]
        letterSpacing=[xs(-0.04->#em)]
        fontWeight=[xs(#400)]
        my=[xs(2)]
        tag=#a>
        {`See source on Github 🔎`->React.string}
      </Typography>
    </Box>
  </Box>
}
