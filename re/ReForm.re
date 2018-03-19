module Helpers = ReForm_Helpers;

module Validation = ReForm_Validation;

module Value = ReForm_Value;

/* Validation types */
let safeHd = lst => List.length(lst) == 0 ? None : Some(List.hd(lst));

let (>>=) = (value, map) =>
  switch value {
  | None => None
  | Some(value) => map(value)
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
    | HandleSubmit
		| ResetFormState;
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

	type onSubmit = {
		values,
		setSubmitting: (bool) => unit,
		setError: (option(string)) => unit,
		resetFormState: unit => unit
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
        ~onSubmit: onSubmit => unit,
        ~onFormStateChange: state => unit=ignore,
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
			| ResetFormState => ReasonReact.UpdateWithSideEffects({...state, values: initialState, errors: [], isSubmitting: false},  (self => onFormStateChange(self.state)))	
      | HandleSubmitting(isSubmitting) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, isSubmitting},
          (self => onFormStateChange(self.state))
        )
      | HandleError(error) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, isSubmitting: false, error},
          (self => onFormStateChange(self.state))
        )
      | SetFieldsErrors(errors) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, isSubmitting: false, errors},
          (self => onFormStateChange(self.state))
        )
      | HandleFieldValidation((field, value)) =>
        ReasonReact.UpdateWithSideEffects(
          {
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
          },
          (self => onFormStateChange(self.state))
        )
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
            self => {
              onSubmit({
								resetFormState: () => self.send(ResetFormState),
                values: state.values,
                setSubmitting:
                  isSubmitting => self.send(HandleSubmitting(isSubmitting)),
                setError: error => self.send(HandleError(error))
							});
              onFormStateChange(self.state);
            }
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
