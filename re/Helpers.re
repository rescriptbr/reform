let handleStringChange = (handleChange, event) =>
  handleChange(
    Value.String(
      ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value,
    ),
  );

let handleIntChange = (handleChange, event) =>
  handleChange(
    Value.Int(
      int_of_string(
        ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value,
      ),
    ),
  );

let handleFloatChange = (handleChange, event) =>
  handleChange(
    Value.Float(
      ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value,
    ),
  );

let handleBoolChange = (handleChange, event) =>
  handleChange(
    Value.Bool(
      ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##checked,
    ),
  );

let handleDomFormSubmit = (handleSubmit, event) => {
  ReactEventRe.Synthetic.preventDefault(event);
  handleSubmit();
};
