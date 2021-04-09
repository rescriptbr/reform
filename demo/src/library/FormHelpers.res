let preventDefault = event => {
  ReactEvent.Form.preventDefault(event)
}

let updateField = (field, fn, event) => fn(field, ReactEvent.Form.target(event)["value"])
