module FormConfig = {
  type occupation =
    | Student
    | Professional
    | Other;

  type field(_) =
    | OccupationDetail: field(string)
    | Occupation: field(option(occupation))
    | Password: field(string)
    | ConfirmPassword: field(string);

  type state = {
    occupationDetail: string,
    occupation: option(occupation),
    password: string,
    confirmPassword: string,
  };

  let get: type value. (state, field(value)) => value =
    (state, field) =>
      switch (field) {
      | Occupation => state.occupation
      | OccupationDetail => state.occupationDetail
      | Password => state.password
      | ConfirmPassword => state.confirmPassword
      };

  let set: type value. (state, field(value), value) => state =
    (state, field, value) =>
      switch (field) {
      | Occupation => {...state, occupation: value}
      | OccupationDetail => {...state, occupationDetail: value}
      | ConfirmPassword => {...state, confirmPassword: value}
      | Password => {...state, password: value}
      };
};

module Form = ReFormNext.Make(FormConfig);

let component = ReasonReact.statelessComponent("Form");

let make = _children => {
  ...component,
  render: _self =>
    <Form
      onSubmit={(~send as _, ~state as _) => ()}
      initialState={
        occupationDetail: "",
        occupation: None,
        confirmPassword: "",
        password: "",
      }
      schema={
        Form.Validation.Schema([|
          Custom(
            OccupationDetail,
            values => values.occupation == None ? Error("Nao vazio") : Valid,
          ),
          Optional(Occupation),
          Optional(OccupationDetail),
        |])
      }>
      ...{
           ({send, state}) =>
             <div>
               {
                 let hasError =
                   state.fieldsState
                   ->Belt.Array.some(((_, state)) =>
                       switch (state) {
                       | Error(_) => true
                       | _ => false
                       }
                     );

                 ReasonReact.null;
               }
             </div>
         }
    </Form>,
};
