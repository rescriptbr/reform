type file = {default: string}

@val external require: string => file = "require"

let reformLogo = require("./reform-logo.svg").default
