// trims off errors due to floating point arithmetic
const precision = 8;
const epsilon = Math.pow(10, precision * -1);
function fixRoundingError(num) {
    return Math.abs(num) <= epsilon ? 0 : Number(num.toPrecision(precision));
}

// helper to format a vector
function printVector(name, vec) {
    return `${name}: ${fixRoundingError(vec[0])}I + ${fixRoundingError(vec[1])}J + ${fixRoundingError(vec[2])}K`;
}