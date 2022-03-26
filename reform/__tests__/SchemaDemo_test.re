open Jest;

module ProfileLenses = {
  type state = {
    email: string,
    age: int,
  };
  type field(_) =
    | Email: field(string)
    | Age: field(int);
  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Email => state.email
      | Age => state.age
      };
  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Email => {...state, email: value}
      | Age => {...state, age: value}
      };
};

test("should validate a correct record", () => {
  open Expect;

  let user = ProfileLenses.{email: "teste@mail.com", age: 18};

  module ProfileValidation = ReForm__ReSchema.Make(ProfileLenses);

  let schema =
    ProfileValidation.Validation.(Schema(email(Email) + int(~min=18, Age)));

  expect(schema |> ProfileValidation.validate(user))
  |> toBe(ReForm__ReSchema.Errors([||]));
});
