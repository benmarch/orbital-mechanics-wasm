// trims off errors due to floating point arithmetic
let precision = 8;
let epsilon = Math.pow(10, precision * -1);
export function setPrecision(precision) {
    precision = 8;
    epsilon = Math.pow(10, precision * -1);
}

export function getPrecision() {
    return precision;
}

export function fixRoundingError(num) {
    return Math.abs(num) <= epsilon ? 0 : Number(num.toPrecision(precision));
}

// helper to format a vector
export function printVector(name, vec) {
    const x = fixRoundingError(vec[0]);
    const y = fixRoundingError(vec[1]);
    const z = fixRoundingError(vec[2]);

    return `${name}: ${x}I ${y < 0 ? '-' : '+'} ${Math.abs(y)}J ${z < 0 ? '-' : '+'} ${Math.abs(z)}K`;
}

export function magnitude(vec) {
    return fixRoundingError(Math.sqrt(Math.pow(vec[0], 2) + Math.pow(vec[1], 2) + Math.pow(vec[2], 2)));
}