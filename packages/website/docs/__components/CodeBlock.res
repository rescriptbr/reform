module DocusaurusCodeBlock = {
  @react.component @module("@theme/CodeBlock")
  external make: (
    ~metastring: string=?,
    ~title: string=?,
    ~className: string,
    ~children: array<string>,
  ) => React.element = "default"
}

let isHiddenComment = line => {
  let regex = %re("/\/\/ hidden-line/")
  !(regex->Js.Re.test_(line))
}

let cleanup = code =>
  code
  ->Belt.Array.map(v =>
    v->Js.String2.split("\n")->Belt.Array.keep(isHiddenComment)->Js.Array2.joinWith("\n")
  )
  ->Belt.Array.keep(v => v !== " ")

@react.component
let make = (~title, ~language, ~metastring=?, ~children) => {
  <DocusaurusCodeBlock ?metastring ?title className={`language-${language}`}>
    {children->cleanup}
  </DocusaurusCodeBlock>
}

let default = make
