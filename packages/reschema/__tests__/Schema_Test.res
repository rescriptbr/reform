open! Jest

module Lenses = {
  type state = {name: string, email: string}

  type rec field<_> = Name: field<string> | Email: field<string>

  let get:
    type value. (state, field<value>) => value =
    (state, field) =>
      switch field {
      | Name => state.name
      | Email => state.email
      }

  let set:
    type value. (state, field<value>, value) => state =
    (state, field, value) =>
      switch field {
      | Name => {...state, name: value}
      | Email => {...state, email: value}
      }
}

module CustomSchema = ReSchema.Make(Lenses)

describe("Schema", (. ()) => {
  it("validateOne", (. ()) => {
    let schema = {
      open CustomSchema.Validation

      schema([
        string(~min=12, ~minError="Invalid name.", Name),
        email(~error="Invalid email", Email),
      ])
    }

    let nameResult = CustomSchema.validateOne(
      ~field=Field(Name),
      ~values={name: "Alonzo", email: "rescript@rescript.com"},
      schema,
    )

    expect(nameResult)->not->toBe(None)

    switch nameResult {
    | None => ()
    | Some((field, error)) => {
        expect(field)->toStrictEqual(Field(Name))->ignore
        expect(error)->toStrictEqual(Error("Invalid name."))->ignore
      }
    }

    let emailResult = CustomSchema.validateOne(
      ~field=Field(Email),
      ~values={name: "Alonzo", email: "test.."},
      schema,
    )

    switch emailResult {
    | None => ()
    | Some((field, error)) => {
        expect(field)->toStrictEqual(Field(Email))->ignore
        expect(error)->toStrictEqual(Error("Invalid email."))->ignore
      }
    }
  })
})
