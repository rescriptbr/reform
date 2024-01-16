## Basic usage

First off, you will need `lenses-ppx` installed, you can do this by following the installation instructions [here](https://github.com/Astrocoders/lenses-ppx).

### Creating the state with lenses-ppx
Before the schema, you need to create the state model using `lenses-ppx`.

```rescript
module Lenses = %lenses(
  type state = {
    name: string,
    email: string,
    age: int,
  }
)
```

### Creating our schema

The next step is to create a custom schema module and build a simple schema using it:
```rescript
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

  [
    StringNonEmpty({
      field: Lenses.Name,
      error: None,
    }),
    IntMin({
      field: Age,
      min: 18,
      error: None,
    }),
  ]
}
```
Now, we can use any function of `MySchema` module to validate our state. In this case, we'll use the `validate` function to validate all fields of the state:
```rescript
let myState = { name: "Alonzo Church", email: "alonzo@gmail.com", age: 12 }

let result = MySchema.validate(myState, schema)

switch result {
| Valid => Js.log("Valid")
| Errors(errors) => Js.log2("Errors =>", errors)
}
```
If you run this code in your browser (or Node) the result probably will be something like: 
```javascript
"Errors => [ [ { _0: 2 }, 'This value must be greater than or equal to 18' ] ]"
```

## Using validators
ReSchema provides some [validators] to create schemas without adding a lot of variants in the schema.
You can use these functions to create validations for `IntMin` and `IntMax` using a single a function `int(...)`. Let's see in practice:

```rescript
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

  schema([
    // Add this
    nonEmpty(Name), 
    int(~min=18, Age),
  ])
}

let result = MySchema.validate({name: "Marcos", email: "", age: 12}, schema)

switch result {
| Valid => Js.log("Valid")
| Errors(errors) => Js.log2("Errors =>", errors)
}
```
> 💡 You can see all available validators [here](https://github.com/rescriptbr/reschema/blob/master/docs/2-api-reference.md#validators).
