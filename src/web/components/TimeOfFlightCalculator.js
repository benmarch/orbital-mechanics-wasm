import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './AdjustableInput.js';
import './Checkbox.js';

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

                .solve-for__timeOfFlight #timeOfFlightInput {
                    display: none;
                }
                .solve-for__timeOfFlight #timeOfFlightValue {
                    display: block;
                }

                .solve-for__timeOfFlight #initialTrueAnomalyInput {
                    display: block;
                }
                .solve-for__timeOfFlight #initialTrueAnomalyValue {
                    display: none;
                }

                .solve-for__timeOfFlight #finalTrueAnomalyInput {
                    display: block;
                }
                .solve-for__timeOfFlight #finalTrueAnomalyValue {
                    display: none;
                }

                .solve-for__finalTrueAnomaly #timeOfFlightInput {
                    display: block;
                }
                .solve-for__finalTrueAnomaly #timeOfFlightValue {
                    display: none;
                }

                .solve-for__finalTrueAnomaly #initialTrueAnomalyInput {
                    display: block;
                }
                .solve-for__finalTrueAnomaly #initialTrueAnomalyValue {
                    display: none;
                }

                .solve-for__finalTrueAnomaly #finalTrueAnomalyInput {
                    display: none;
                }
                .solve-for__finalTrueAnomaly #finalTrueAnomalyValue {
                    display: block;
                }

                .solve-for__initialTrueAnomaly #timeOfFlightInput {
                    display: block;
                }
                .solve-for__initialTrueAnomaly #timeOfFlightValue {
                    display: none;
                }

                .solve-for__initialTrueAnomaly #initialTrueAnomalyInput {
                    display: none;
                }
                .solve-for__initialTrueAnomaly #initialTrueAnomalyValue {
                    display: block;
                }

                .solve-for__initialTrueAnomaly #finalTrueAnomalyInput {
                    display: block;
                }
                .solve-for__initialTrueAnomaly #finalTrueAnomalyValue {
                    display: none;
                }

            </style>


            <h3>Please select an orbit: </h3>
            <div id="errorMessage"></div>
            <orbit-selector max-orbits="1" on:orbitselected="handleOrbitSelected"></orbit-selector>

            <div id="selectedOrbits"></div>

            <h3>Solve For:</h3>
            <select id="solveForQuantity" on:change="handleSelect">
                <option value="timeOfFlight" selected>Time of Flight</option>
                <option value="finalTrueAnomaly">Final True Anomaly (ν<sub>f</sub>)</option>
                <option value="initialTrueAnomaly">Initial True Anomaly (ν<sub>i</sub>)</option>
            </select>

            <dl id="dataContainer" class="solve-for__timeOfFlight">
                <dt>Initial True Anomaly (ν<sub>i</sub>):</dt>
                <dd>
                    <span id="initialTrueAnomalyValue">&nbsp;</span>
                    <adjustable-input id="initialTrueAnomalyInput" type="range" min="0" max="360" value="0" on:input="handleChange"></adjustable-input>
                </dd>

                <dt>Final True Anomaly (ν<sub>f</sub>):</dt>
                <dd>
                    <span id="finalTrueAnomalyValue">&nbsp;</span>
                    <adjustable-input id="finalTrueAnomalyInput" type="range" min="0" max="360" value="0" on:input="handleChange"></adjustable-input>
                </dd>

                <dt>Time of Flight (TOF):</dt>
                <dd>
                    <span id="timeOfFlightValue">&nbsp;</span>
                    <adjustable-input id="timeOfFlightInput" type="range" min="0" value="0" on:input="handleChange"></adjustable-input>
                </dd>

                <dt>Rate of Mean Motion Change (∆n/∆t):</dt>
                <dd>
                    <adjustable-input id="aRateOfChange" type="range" min="0" max="10" step="0.01" value="0" on:input="handleMeanMotionRateOfChange"></adjustable-input>
                </dd>

                <dt>Mean Motion (n):</dt>
                <dd id="meanMotion">&nbsp;</dd>

                <dt>Initial Eccentric Anomaly (E<sub>i</sub>):</dt>
                <dd id="initialEccentricAnomaly">&nbsp;</dd>

                <dt>Final Eccentric Anomaly (E<sub>f</sub>):</dt>
                <dd id="finalEccentricAnomaly">&nbsp;</dd>

                <dt>Initial Mean Anomaly (M<sub>i</sub>):</dt>
                <dd id="initialMeanAnomaly">&nbsp;</dd>

                <dt>Final Mean Anomaly (M<sub>f</sub>):</dt>
                <dd id="finalMeanAnomaly">&nbsp;</dd>
            </dl>
        `;
    }

    constructor() {
        super();

        this.solveForQuantity = 'timeOfFlight';
        this.meanMotionRateOfChange = 0;
    }

    renderSelectedOrbits(selectedOrbits) {
        const orbitName = selectedOrbits[0] ? selectedOrbits[0].name : '&lt;Select an Orbit&gt;';

        this.selectedOrbitsElement.innerHTML = `Orbit: ${orbitName}`;
    }

    handleOrbitSelected({detail: selectedOrbits}) {
        this.renderSelectedOrbits(selectedOrbits);

        if (selectedOrbits.length === 1) {
            this.setOrbit(selectedOrbits[0].orbit);
        }
    }

    handleSelect() {
        this.dataContainerElement.classList.remove(`solve-for__${this.solveForQuantity}`)
        this.solveForQuantity = this.solveForQuantityElement.value;
        this.dataContainerElement.classList.add(`solve-for__${this.solveForQuantity}`)
    }

    handleChange(event) {
        const {id, value} = event.target;

        switch (id) {
            case 'timeOfFlightInput':
                this.timeOfFlight = value;
                break;
            case 'finalTrueAnomalyInput':
                this.finalTrueAnomaly = value * Math.PI / 180;
                break;
            case 'initialTrueAnomalyInput':
                this.initialTrueAnomaly = value * Math.PI / 180;
                break;
        }
        this.recalculate();
    }

    handleMeanMotionRateOfChange() {
        this.meanMotionRateOfChange = Number(this.aRateOfChangeElement.value);
        this.recalculate();
    }

    setOrbit(orbit) {
        this.timeOfFlightCalculator = new Module.TimeOfFlightCalculator(orbit);
    }

    recalculate() {
        if (this.timeOfFlightCalculator) {
            this.timeOfFlightCalculator.setMeanMotionRateOfChange(this.meanMotionRateOfChange);
            switch (this.solveForQuantity) {
                case 'timeOfFlight':
                    this.timeOfFlightCalculator.calculateTimeOfFlight(this.initialTrueAnomaly || 0, this.finalTrueAnomaly || 0);
                    break;
                case 'finalTrueAnomaly':
                    this.timeOfFlightCalculator.calculateFinalTrueAnomaly(this.initialTrueAnomaly || 0, this.timeOfFlight || 0);
                    break;
                case 'initialTrueAnomaly':
                    this.timeOfFlightCalculator.calculateInitialTrueAnomaly(this.finalTrueAnomaly || 0, this.timeOfFlight || 0);
                    break;
            }
            this.render();
        }
    }

    render() {
        this.meanMotionElement.innerHTML = fixRoundingError(this.timeOfFlightCalculator.meanMotion || 0) + ' s<sup>-1</sup>';
        this.initialEccentricAnomalyElement.innerHTML = fixRoundingError(this.timeOfFlightCalculator.initialEccentricAnomaly || 0) + ' rad';
        this.finalEccentricAnomalyElement.innerHTML = fixRoundingError(this.timeOfFlightCalculator.finalEccentricAnomaly || 0) + ' rad';
        this.initialMeanAnomalyElement.innerHTML = fixRoundingError(this.timeOfFlightCalculator.initialMeanAnomaly || 0) + ' rad';
        this.finalMeanAnomalyElement.innerHTML = fixRoundingError(this.timeOfFlightCalculator.finalMeanAnomaly || 0) + ' rad';

        this.timeOfFlightValueElement.innerHTML = '&nbsp;';
        this.finalTrueAnomalyValueElement.innerHTML = '&nbsp;';
        this.initialTrueAnomalyValueElement.innerHTML = '&nbsp;';

        switch (this.solveForQuantity) {
            case 'timeOfFlight':
                this.timeOfFlightValueElement.innerHTML = fixRoundingError(this.timeOfFlightCalculator.timeOfFlight) + ' s';
                break;
            case 'finalTrueAnomaly':
                this.finalTrueAnomalyValueElement.innerHTML = fixRoundingError(this.timeOfFlightCalculator.finalTrueAnomaly * 180/Math.PI) + ' degrees';
                break;
            case 'initialTrueAnomaly':
                this.initialTrueAnomalyValueElement.innerHTML = fixRoundingError(this.timeOfFlightCalculator.initialTrueAnomaly * 180/Math.PI) + ' degrees';
                break;
        }
    }
}

window.customElements.define('time-of-flight-calculator', TimeOfFlightCalculator);