// trims off errors due to floating point arithmetic
let precision = 8;
let epsilon = Math.pow(10, precision * -1);
function setPrecision(precision) {
    precision = 8;
    epsilon = Math.pow(10, precision * -1);
}
function fixRoundingError(num) {
    return Math.abs(num) <= epsilon ? 0 : Number(num.toPrecision(precision));
}

// helper to format a vector
function printVector(name, vec) {
    return `${name}: ${fixRoundingError(vec[0])}I + ${fixRoundingError(vec[1])}J + ${fixRoundingError(vec[2])}K`;
}

function magnitude(vec) {
    return fixRoundingError(Math.sqrt(Math.pow(vec[0], 2) + Math.pow(vec[1], 2) + Math.pow(vec[2], 2)));
}

// borrowed from Polymer, creates a template element from html string
function html(strings, ...values) {
    const template = document.createElement('template');
    template.innerHTML = values.reduce((acc, v, idx) =>
    acc + v + strings[idx + 1], strings[0]);
    return template;
};