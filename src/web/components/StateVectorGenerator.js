import Component, { html } from '../Component.js';
import { setPrecision, getPrecision, fixRoundingError, printVector, magnitude } from '../utils.js';
import './AdjustableInput.js';
import './SaveOrbitButton.js';

class StateVectorGenerator extends Component {
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

                #velocity {
                    margin-bottom: var(--gutter-width);
                }

                #saveOrbitButton {
                    margin-bottom: var(--gutter-width);
                }
            </style>

            <save-orbit-button id="saveOrbitButton" on:orbitsaved="handleOrbitSaved"></save-orbit-button>

            <div id="radius"></div>
            <div id="velocity"></div>

            <form id="elementsForm" name="elements">
                <span>Semimajor Axis (a):</span>
                <adjustable-input name="a" type="range" min="1" max="100000" value="6378"></adjustable-input>

                <span>Eccentricity (e):</span>
                <adjustable-input name="e" type="range" min="0" max="0.99" step="0.01" value="0"></adjustable-input>

                <span>Inclination (i):</span>
                <adjustable-input name="i" type="range" min="0" max="180" value="0"></adjustable-input>

                <span>Right Ascension of the Ascending Node (Ω):</span>
                <adjustable-input name="Om" type="range" min="0" max="360" value="0"></adjustable-input>

                <span>Argument of Perigee (ω):</span>
                <adjustable-input name="o" type="range" min="0" max="360" value="0"></adjustable-input>

                <span>True Anomaly (ν):</span>
                <adjustable-input name="nu" type="range" min="0" max="360" value="0"></adjustable-input>

                <span>Argument of Latitude (u):</span>
                <adjustable-input name="u" type="range" min="0" max="360" value="0"></adjustable-input>

                <span>Longitude of Perigee (∏):</span>
                <adjustable-input name="Pi" type="range" min="0" max="360" value="0"></adjustable-input>

                <span>True Longitude (l):</span>
                <adjustable-input name="l" type="range" min="0" max="360" value="0"></adjustable-input>
            </form>
        `;
    }

    static get angleElements() { return ['i', 'o', 'Om', 'nu', 'u', 'Pi', 'l']; }

    constructor() {
        super();

        this.orbit = new Module.Orbit();
        this.fields = [];
        this.elements = {
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

        this.shadowRoot.querySelectorAll('adjustable-input').forEach(input => {
            this.fields.push(input);
            input.addEventListener('input', this.handleInput.bind(this));
        });
    }

    connectedCallback() {
        this.updateWithOrbitalElements(this.elements);
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
            } else if (Array.isArray(element)) {
                value = element[0];
            }

            setPrecision(Math.min(String(value).replace(/[\.\-]/g, '').length, getPrecision()));
        })

        this.regenerate();
        this.dispatchEvent(new Event('input'));
    }

    handleOrbitSaved() {
        const mu = this.orbit.mu;
        Module._free(this.orbit);
        this.orbit = new Module.Orbit(mu);
        this.regenerate();
    }

    updateWithOrbitalElements(elements) {
        this.fields.forEach(field => {
            // convert to degrees if its an angle
            const multiplier = StateVectorGenerator.angleElements.includes(field.name) ? 180/Math.PI : 1;

            if (typeof elements[field.name] === 'number') {
                field.setAttribute('value', elements[field.name] * multiplier);
            } else if (Array.isArray(elements[field.name])) {
                field.setAttribute('value', magnitude(elements[field.name]) * multiplier);
            }
        });

        this.elements = elements;
        this.regenerate();
    }

    setMu(mu) {
        this.orbit.setMu(mu);
        this.regenerate();
    }

    getStateVectors() {
        this.orbit.updateFromOrbitalElements(this.elements);
        return this.orbit.stateVectors;
    }

    regenerate() {
        const stateVectors = this.getStateVectors();

        this.saveOrbitButtonElement.setOrbit(this.orbit);

        this.radiusElement.innerHTML = 'R: ' + printVector(stateVectors.position);
        this.velocityElement.innerHTML = 'V: ' + printVector(stateVectors.velocity);

        this.value = stateVectors;
    }
}

window.customElements.define('state-vector-generator', StateVectorGenerator);
