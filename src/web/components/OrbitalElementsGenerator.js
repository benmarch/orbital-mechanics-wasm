import Component, { html } from '../Component.js';
import { fixRoundingError, magnitude } from '../utils.js';

import './TextInput.js';
import './SaveOrbitButton.js';

export default class OrbitalElementsGenerator extends Component {
    static get template() {
        return html`
            <style>
                dt, dd {
                    float: left;
                    width: 50%;
                    margin: 0;
                }

                text-input {
                    display: inline-block;
                    width: 150px;
                }

                #radius {
                    margin-bottom: var(--gutter-width);
                }

                #saveOrbitButton {
                    margin-bottom: var(--gutter-width);
                }
            </style>

            <save-orbit-button id="saveOrbitButton"></save-orbit-button>

            <form id="stateVectorForm" name="stateVectors">
                <div id="radius">
                    R:
                    <text-input type="number" name="position_i" value="0"></text-input>I +
                    <text-input type="number" name="position_j" value="0"></text-input>J +
                    <text-input type="number" name="position_k" value="0"></text-input>K
                </div>
                <div id="velocity">
                    V:
                    <text-input type="number" name="velocity_i" value="0"></text-input>I +
                    <text-input type="number" name="velocity_j" value="0"></text-input>J +
                    <text-input type="number" name="velocity_k" value="0"></text-input>K
                </div>
            </form>
            <br>

            <div id="elements"></div>
        `;
    }

    constructor() {
        super();

        this.orbit = new Module.Orbit();
        this.fields = [];
        this.stateVectors = {
            position: [0, 0, 0],
            velocity: [0, 0, 0]
        };
        this.dimensionMap = {
            i: 0,
            j: 1,
            k: 2
        };

        this.shadowRoot.querySelectorAll('text-input').forEach(input => {
            input.addEventListener('input', this.handleInput.bind(this));
            this.fields.push(input);
        });
    }

    connectedCallback() {
        this.regenerate();

        this.saveOrbitButtonElement.setOrbit(this.orbit);
    }

    handleInput(event) {
        const { name, value } = event.target;
        const [state, dimension] = name.split('_');

        this.stateVectors[state][this.dimensionMap[dimension]] = Number(value);

        this.regenerate();
        this.dispatchEvent(new Event('input'));
    }

    populateFields() {
        this.fields.forEach(input => {
            const [state, dimension] = input.name.split('_');

            input.value = this.stateVectors[state][this.dimensionMap[dimension]];
        })
    }

    updateWithStateVectors(stateVectors) {
        this.stateVectors = stateVectors;

        this.populateFields();
        this.regenerate();
    }

    getElements() {
        return this.orbit.elements;
    }

    getDisplayValues(elements) {
        return {
            a: (fixRoundingError(elements.a) || 0) + ' km',
            e: fixRoundingError(magnitude(elements.e)) || 0,
            i: (fixRoundingError(elements.i * 180/Math.PI) || 0) + ' degrees',
            Om: this.orbit.isEquatorial ? 'N/A' : (fixRoundingError(elements.Om * 180/Math.PI) || 0) + ' degrees',
            o: this.orbit.isEquatorial || this.orbit.isCircular ? 'N/A' : (fixRoundingError(elements.o * 180/Math.PI) || 0) + ' degrees',
            nu: this.orbit.isCircular ? 'N/A' : (fixRoundingError(elements.nu * 180/Math.PI) || 0) + ' degrees',
            u: this.orbit.isCircular && !this.orbit.isEquatorial ? (fixRoundingError(elements.u * 180/Math.PI) || 0) + ' degrees' : 'N/A',
            Pi: this.orbit.isEquatorial && !this.orbit.isCircular ? (fixRoundingError(elements.Pi * 180/Math.PI) || 0) + ' degrees' : 'N/A',
            l: this.orbit.isCircular && this.orbit.isEquatorial ? fixRoundingError(elements.l * 180/Math.PI) || 0 + ' degrees' : 'N/A'
        };
    }

    regenerate() {
        this.orbit.updateFromStateVectors(this.stateVectors);

        this.value = this.orbit.elements;
        var elements = this.getDisplayValues(this.value);


        this.elementsElement.innerHTML = `
            <dl>
                <dt>Semimajor Axis (a):</dt>
                <dd>${elements.a}</dd>

                <dt>Eccentricity (e):</dt>
                <dd>${elements.e}</dd>

                <dt>Inclination (i):</dt>
                <dd>${elements.i}</dd>

                <dt>Right Ascension (Ω):</dt>
                <dd>${elements.Om}</dd>

                <dt>Argument of Perigee (ω):</dt>
                <dd>${elements.o}</dd>

                <dt>True Anomaly (ν):</dt>
                <dd>${elements.nu}</dd>

                <dt>Argument of Latitude (u):</dt>
                <dd>${elements.u}</dd>

                <dt>Longitude of Perigee (∏):</dt>
                <dd>${elements.Pi}</dd>

                <dt>True Longitude (l):</dt>
                <dd>${elements.l}</dd>
            </dl>
        `;
    }
}

window.customElements.define('orbital-elements-generator', OrbitalElementsGenerator);
