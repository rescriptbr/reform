module ProfileLenses = [%lenses
  type state = {
    name: string,
    age: int,
  }
];

open ProfileLenses;

module Schema = ReSchema.Make(ProfileLenses);

let schema: Schema.Validation.schema(option(unit)) = {
  Schema.Validation.(
    Schema(
      string(~min=12, Name)
      + int(~min=18, ~minError="Too young", ~max=45, Age),
    )
  );
};

[@react.component]
let make = () => {
  let (profile, setProfile) = React.useState(_ => {name: "", age: 0});
  let (validation, setValidation) = React.useState(_ => None);

  let validateData = () => {
    setValidation(_ => Some(Schema.validate(profile, schema)));
  };

  let handleChange = (field, event) => {
    let value = ReactEvent.Form.target(event)##value;
    let newProfile = profile->ProfileLenses.set(field, value);

    setProfile(_ => newProfile);
  };

  <div>
    <input
      onBlur={_ => validateData()}
      placeholder="Nome"
      onChange={handleChange(Name)}
    />
    <input
      onBlur={_ => validateData()}
      placeholder="Idade"
      onChange={handleChange(Age)}
    />
    {switch (validation) {
     | None => React.null
     | Some(Valid) => <p> "Form is like totally valid"->React.string </p>
     | Some(Errors(errors)) =>
       <>
         <p> "Form is like not valid at all"->React.string </p>
         <ul>
           {errors
            ->Belt.Array.map(((_field, error)) => {
                <li> error->React.string </li>
              })
            ->React.array}
         </ul>
       </>
     }}
  </div>;
};
