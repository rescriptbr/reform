let handleChange = (handleChange, event) =>
  handleChange(ReactEvent.Form.target(event)##value);

let handleSubmit = (handleSubmit, event) => {
  ReactEvent.Synthetic.preventDefault(event);
  handleSubmit();
};
