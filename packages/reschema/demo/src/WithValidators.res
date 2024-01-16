module Lenses = %lenses(
  type state = {
    name: string,
    email: string,
    age: int,
  }
)

module MySchema = ReSchema.Make(Lenses)

let schema = {
  open MySchema.Validation

  schema([nonEmpty(Name), int(~min=18, Age)])
}

let result = MySchema.validate({name: "Marcos", email: "", age: 12}, schema)

switch result {
| Valid => Js.log("Valid")
| Errors(errors) => Js.log2("Errors =>", errors)
}
