/* Validation types */
let safeHd = lst => List.length(lst) == 0 ? None : Some(List.hd(lst));

let (>>=) = (value, map) =>
  switch value {
  | None => None
  | Some(value) => map(value)
  };

module Validation = {
  module I18n = {
    type dictionary = {
      required: string,
      email: string
    };
    let ptBR = {
      required: {j|Campo obrigatório|j},
      email: {j|Email inválido|j}
    };
    let en = {required: "Field is required", email: "Invalid email"};
  };
  type validation('values) =
    | Required
    | Email
    | Custom('values => option(string));
  let getValidationError =
      ((_, validator), ~values, ~value, ~i18n: I18n.dictionary) =>
    switch validator {
    | Required => String.length(value) < 1 ? Some(i18n.required) : None
    | Custom(fn) => fn(values)
    | Email =>
      Js.Re.test(value, [%bs.re {|/\S+@\S+\.\S+/|}]) ? None : Some(i18n.email)
    };
};

module Helpers = {
  let handleDomFormChange = (handleChange, event) =>
    handleChange(
      ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value
    );
  let handleDomFormSubmit = (handleSubmit, event) => {
    ReactEventRe.Synthetic.preventDefault(event);
    handleSubmit();
  };
};

module Value = {
  type t = string;
};

module type Config = {
  type state;
  type fields;
  /* (fieldName, getter, setter) */
  let lens: list((fields, state => Value.t, (state, Value.t) => state));
};

module Create = (Config: Config) => {
  /* TODO: Make a variant out of this */
  type value = Value.t;
  /* Form actions */
  type action =
    | HandleSubmitting(bool)
    | SetFieldsErrors(list((Config.fields, option(string))))
    | HandleFieldValidation((Config.fields, value))
    | HandleError(option(string))
    | HandleChange((Config.fields, value))
    | HandleSubmit;
  type values = Config.state;
  type schema = list((Config.fields, Validation.validation(values)));
  module Field = {
    let getFieldLens:
      Config.fields =>
      (
        Config.fields,
        Config.state => value,
        (Config.state, value) => Config.state
      ) =
      field =>
        /* TODO handle exception */
        Config.lens |> List.find(((fieldName, _, _)) => fieldName === field);
    let validateField:
      (Config.fields, values, value, schema, Validation.I18n.dictionary) =>
      option(string) =
      (field, values, value, schema, i18n) =>
        schema
        |> List.filter(((fieldName, _)) => fieldName === field)
        |> safeHd
        >>= (
          fieldSchema =>
            Validation.getValidationError(fieldSchema, ~values, ~value, ~i18n)
        );
    let handleChange: ((Config.fields, value), values) => values =
      ((field, value), values) => {
        let (_, _, setter) = getFieldLens(field);
        setter(values, value);
      };
  };
  type state = {
    values,
    isSubmitting: bool,
    errors: list((Config.fields, option(string))),
    error: option(string)
  };
  /* Type of what is given to the children */
  type reform = {
    form: state,
    handleChange: (Config.fields, value) => unit,
    handleGlobalValidation: option(string) => unit,
    handleSubmit: unit => unit,
    getErrorForField: Config.fields => option(string)
  };
  let component = ReasonReact.reducerComponent("ReForm");
  let make =
      (
        ~onSubmit:
           (
             values,
             ~setSubmitting: ReasonReact.Callback.t(bool),
             ~setError: ReasonReact.Callback.t(option(string))
           ) =>
           unit,
        ~validate: values => option(string)=(_) => None,
        ~initialState: Config.state,
        ~schema: schema,
        ~i18n: Validation.I18n.dictionary=Validation.I18n.en,
        children
      ) => {
    ...component,
    initialState: () => {
      values: initialState,
      error: None,
      isSubmitting: false,
      errors: []
    },
    reducer: (action, state) =>
      switch action {
      | HandleSubmitting(isSubmitting) =>
        ReasonReact.Update({...state, isSubmitting})
      | HandleError(error) =>
        ReasonReact.Update({...state, isSubmitting: false, error})
      | SetFieldsErrors(errors) =>
        ReasonReact.Update({...state, isSubmitting: false, errors})
      | HandleFieldValidation((field, value)) =>
        ReasonReact.Update({
          ...state,
          errors:
            state.errors
            |> List.filter(((fieldName, _)) => fieldName !== field)
            |> List.append([
                 (
                   field,
                   Field.validateField(
                     field,
                     state.values,
                     value,
                     schema,
                     i18n
                   )
                 )
               ])
        })
      | HandleChange((field, value)) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, values: Field.handleChange((field, value), state.values)},
          (
            self =>
              self.reduce((_) => HandleFieldValidation((field, value)), ())
          )
        )
      | HandleSubmit =>
        ReasonReact.UpdateWithSideEffects(
          {...state, isSubmitting: true},
          (
            self =>
              onSubmit(
                state.values,
                ~setSubmitting=
                  isSubmitting => self.send(HandleSubmitting(isSubmitting)),
                ~setError=error => self.send(HandleError(error))
              )
          )
        )
      },
    render: self => {
      let handleChange = (field, value) =>
        self.send(HandleChange((field, value)));
      let handleGlobalValidation = error => self.send(HandleError(error));
      let handleFormSubmit = (_) => self.send(HandleSubmit);
      let handleSubmit = (_) => {
        let globalValidationError = validate(self.state.values);
        let fieldsValidationErrors =
          schema
          |> List.map(((fieldName, _)) => {
               let (_, getter, _) = Field.getFieldLens(fieldName);
               (
                 fieldName,
                 Field.validateField(
                   fieldName,
                   self.state.values,
                   getter(self.state.values),
                   schema,
                   i18n
                 )
               );
             })
          |> List.filter(((_, fieldError)) => fieldError !== None);
        self.send(SetFieldsErrors(fieldsValidationErrors));
        handleGlobalValidation(globalValidationError);
        globalValidationError === None
        && List.length(fieldsValidationErrors) == 0 ?
          handleFormSubmit() : ignore();
      };
      let getErrorForField: Config.fields => option(string) =
        field =>
          self.state.errors
          |> List.filter(((fieldName, _)) => fieldName === field)
          |> List.map(((_, error)) => error)
          |> safeHd
          >>= (i => i);
      children({
        form: self.state,
        handleChange,
        handleSubmit,
        handleGlobalValidation,
        getErrorForField
      });
    }
  };
};
