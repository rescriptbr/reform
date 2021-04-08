let rootElement = ReactDOM.querySelector("#root")

GlobalStyles.addGlobalStyles()

let () = switch rootElement {
| Some(element) => ReactDOM.render(<App />, element)
| None => Js.log("#root element not found")
}
