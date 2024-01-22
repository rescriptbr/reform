module Lenses = %lenses(
  type state = {
    name: string,
    email: string,
    age: int,
  }
)

module MyForm = ReForm.Make(Lenses)

let initialState: Lenses.state = {
  name: "",
  email: "",
  age: 0,
}

let useMyForm = () => {
  let handleSubmit = ({state}: MyForm.onSubmitAPI) => {
    Js.log2("Values =>", state.values)
    None
  }

  let form = MyForm.use(
    ~initialState,
    ~onSubmit=handleSubmit,
    ~schema={
      open MyForm.Validation

      schema([string(~min=3, Name), int(~min=18, Age)])
    },
    (),
  )

  form
}
