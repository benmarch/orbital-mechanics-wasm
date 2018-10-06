class StateVectorGenerator extends HTMLElement {
    static get template() {
        return html`
            <style>
                [name="e"][value="0"] ~ [name="o"],
                [name="e"][value="0"] ~ [name="Pi"],
                [name="e"][value="0"] ~ [name="nu"],
                [name="e"][value="0"] ~ [name="l"],
                [name="i"][value="0"] ~ [name="Om"],
                [name="i"][value="0"] ~ [name="o"],
                [name="i"][value="0"] ~ [name="u"],
                [name="i"][value="0"] ~ [name="l"],
                [name="e"]:not([value="0"]) ~ [name="i"]:not([value="0"]) ~ [name="u"],
                [name="e"]:not([value="0"]) ~ [name="i"]:not([value="0"]) ~ [name="Pi"],
                [name="e"]:not([value="0"]) ~ [name="i"]:not([value="0"]) ~ [name="l"]{
                    pointer-events: none;
                    opacity: .3;
                }

                [name="e"][value="0"] ~ [name="i"][value="0"] ~ [name="l"] {
                    pointer-events: all;
                    opacity: 1;
                }

            </style>

            <div id="radius"></div>
            <div id="velocity"></div>
            <br>

            <form id="elementsForm" name="elements">
                <span>Semimajor Axis (a):</span>
                <adjustable-input name="a" type="range" min="1" max="100000" value="6378"></adjustable-input>

                <span>Eccentricity (e):</span>
                <adjustable-input name="e" type="range" min="0" max="0.99" step="0.01"></adjustable-input>

                <span>Inclination (i):</span>
                <adjustable-input name="i" type="range" min="0" max="180"></adjustable-input>

                <span>Right Ascension of the Ascending Node (Ω):</span>
                <adjustable-input name="Om" type="range" min="0" max="360"></adjustable-input>

                <span>Argument of Perigee (o):</span>
                <adjustable-input name="o" type="range" min="0" max="360"></adjustable-input>

                <span>True Anomaly (nu):</span>
                <adjustable-input name="nu" type="range" min="0" max="360"></adjustable-input>

                <span>Argument of Latitude (u):</span>
                <adjustable-input name="u" type="range" min="0" max="360"></adjustable-input>

                <span>Longitude of Perigee (∏):</span>
                <adjustable-input name="Pi" type="range" min="0" max="360"></adjustable-input>

                <span>True Longitude (l):</span>
                <adjustable-input name="l" type="range" min="0" max="360"></adjustable-input>
            </form>
        `;
    }

    static get angleElements() { return ['i', 'o', 'Om', 'nu', 'u', 'Pi', 'l']; }

    orbit = new Module.Orbit();
    radiusElem;
    velocityElem;
    fields = [];

    elements = {
        a: 6378,
        e: [0, 0, 0],
        i: 0,
        o: 0,
        Om: 0,
        nu: 0,
        u: 0,
        Pi: 0,
        l: 0
    };

    constructor() {
        super();

        const shadowRoot = this.attachShadow({mode:'open'});
        shadowRoot.appendChild(document.importNode(StateVectorGenerator.template.content, true));

        this.radiusElem = shadowRoot.getElementById('radius');
        this.velocityElem = shadowRoot.getElementById('velocity');

        shadowRoot.querySelectorAll('adjustable-input').forEach(input => {
            this.fields.push(input);
            input.addEventListener('input', this.handleInput.bind(this));
        });
    }

    connectedCallback() {
        this.regenerate();
    }

    handleInput(event) {
        const { name, value } = event.target;

        if (name === 'e') {
            this.elements.e = [Number(value), 0, 0]; // we only need the magnitude, but this should be fixed
        } else {
            this.elements[name] = StateVectorGenerator.angleElements.includes(name) ? Number(value) * Math.PI/180 : Number(value);
        }

        Object.values(this.elements).forEach(element => {
            let value;
            if (typeof element === 'number') {
                value = element;
            } else if (typeof element === 'array') {
                value = element[0];
            }

            setPrecision(Math.min(String(value).replace(/[\.\-]/g, '').length, precision));
        })

        this.regenerate();
        this.dispatchEvent(new Event('input'));
    }

    updateWithOrbitalElements(elements) {
        this.fields.forEach(field => {
            let value;
            if (typeof elements[field.name] === 'number') {
                value = elements[field.name];
            } else {
                value = magnitude(elements[field.name]);
            }

            field.setAttribute('value', StateVectorGenerator.angleElements.includes(field.name) ? value * 180/Math.PI : value);
        })

        this.elements = elements;
        this.regenerate();
    }

    getStateVectors() {
        this.orbit.updateFromOrbitalElements(this.elements);
        return this.orbit.stateVectors;
    }

    regenerate() {
        var stateVectors = this.getStateVectors();

        this.radiusElem.innerHTML = printVector('R', stateVectors.position);
        this.velocityElem.innerHTML = printVector('V', stateVectors.velocity);

        this.value = stateVectors;
    }
}

window.customElements.define('state-vector-generator', StateVectorGenerator);
