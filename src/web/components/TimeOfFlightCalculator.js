import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './AdjustableInput.js';

class TimeOfFlightCalculator extends Component {
    static get template() {
        return html`
            <style>
                dt, dd {
                    float: left;
                    width: 50%;
                    margin: 0;
                }

                dt {
                    clear: both;
                }

                #dataContainer {
                    width: 400px;
                }

                #errorMessage {
                    color: red;
                }
            </style>


            <h3>Please select an orbit: </h3>
            <div id="errorMessage"></div>
            <orbit-selector max-orbits="1" on:orbitselected="handleOrbitSelected"></orbit-selector>

            <div id="selectedOrbits"></div>

            <dl id="dataContainer">
                <dt>Initial True Anomaly:</dt>
                <dd><adjustable-input id="initialTrueAnomaly" type="range" min="0" max="360" value="0" on:input="handleChange"></adjustable-input></dd>

                <dt>Initial True Anomaly:</dt>
                <dd><adjustable-input id="finalTrueAnomaly" type="range" min="0" max="360" value="0" on:input="handleChange"></adjustable-input></dd>

                <dt>Time of Flight =</dt>
                <dd id="timeOfFlight">&nbsp;</dd>
            </dl>
        `;
    }

    renderSelectedOrbits(selectedOrbits) {
        const orbitName = selectedOrbits[0] ? selectedOrbits[0].name : '&lt;Select an Orbit&gt;';

        this.selectedOrbitsElement.innerHTML = `Orbit: ${orbitName}`;
    }

    handleOrbitSelected({detail: selectedOrbits}) {
        this.renderSelectedOrbits(selectedOrbits);

        if (selectedOrbits.length === 1) {
            this.setOrbits(selectedOrbits[0].orbit);
        }
    }

    handleChange(event) {
        const {id, value} = event.target;
        this[id] = value * Math.PI / 180;

        if (this.timeOfFlightCalculator) {
            this.tof = this.timeOfFlightCalculator.calculateTimeOfFlight(this.initialTrueAnomaly || 0, this.finalTrueAnomaly || 0).tof;
            this.render();
        }
    }

    setOrbits(orbit) {
        this.timeOfFlightCalculator = new Module.TimeOfFlightCalculator(orbit);
    }

    render() {
        this.timeOfFlightElement.innerHTML = fixRoundingError(this.tof || 0) + ' s';
    }
}

window.customElements.define('time-of-flight-calculator', TimeOfFlightCalculator);