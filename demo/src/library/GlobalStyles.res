open CssJs

let addGlobalStyles = () => {
  global(. "*", [fontFamily("'Poppins', sans-serif"->#custom), boxSizing(#borderBox)])

  global(. "html", [fontSize(10->px)])

  global(.
    "html, body, #root",
    [padding(zero), margin(zero), height(100.0->pct), width(100.0->pct)],
  )
}
