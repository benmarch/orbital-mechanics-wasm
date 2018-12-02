import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import {fixRoundingError, printVector} from '../utils.js';

import './OrbitSelector.js';
import './AdjustableInput.js';
import './Checkbox.js';

class LaunchRequirementCalculator extends Component {
    static get template() {
        return html`
            <style>
                :host {
                    display: grid;
                    column-gap: var(--gutter-width);
                    grid-template-columns: 1fr 1fr;
                    grid-template-areas: "left right";
                }

                dt, dd {
                    float: left;
                    width: 50%;
                    margin: 0;
                }

                dt {
                    clear: both;
                }

                #left {
                    grid-area: left;
                }

                #right {
                    grid-area: right;
                }

                #errorMessage {
                    color: red;
                }
            </style>

            <section id="left">
                <h3>Please select an orbit: </h3>
                <div id="errorMessage"></div>
                <orbit-selector max-orbits="1" on:orbitselected="handleOrbitSelected"></orbit-selector>

                <div id="selectedOrbits"></div>

                <h4>Launch Site and Losses:</h4>
                <dl>
                    <dt>L<sub>0</sub> (degrees)</dt>
                    <dd><text-input type="number" id="lsL0"></text-input></dd>

                    <dt>LST (degrees)</dt>
                    <dd><text-input type="number" id="lsLST"></text-input></dd>

                    <dt>Altitude (km)</dt>
                    <dd><text-input type="number" id="lsAlt"></text-input></dd>

                    <dt>∆V<sub>losses</sub> (km/s):</dt>
                    <dd><text-input type="number" id="deltaVLosses"></text-input></dd>
                </dl>
                <om-button on:click="handleLaunchSiteSet">Calculate</om-button>
            </section>

            <section id="right">
                <h4>Launch Requirements:</h4>
                <dl id="dataContainer">
                    <dt>Velocity of Launch Site:</dt>
                    <dd id="vLS"></dd>

                    <dt>Velocity Loss Due to Gravity:</dt>
                    <dd id="vLG"></dd>

                    <dt>Velocity at Burnout:</dt>
                    <dd id="vBO"></dd>

                    <dt>∆V<sub>needed</sub>:</dt>
                    <dd id="deltaVNeeded"></dd>

                    <dt>∆V<sub>design</sub>:</dt>
                    <dd id="deltaVDesign"></dd>
                </dl>
            </section>
        `;
    }

    renderSelectedOrbits(selectedOrbits) {
        const orbitName = selectedOrbits[0] ? selectedOrbits[0].name : '&lt;Select an Orbit&gt;';

        this.selectedOrbitsElement.innerHTML = `Orbit: ${orbitName}`;
    }

    handleOrbitSelected({detail: selectedOrbits}) {
        this.renderSelectedOrbits(selectedOrbits);

        if (selectedOrbits.length === 1) {
            this.orbit = selectedOrbits[0].orbit;
            this.calculate();
        }
    }

    handleLaunchSiteSet() {
        this.launchSite = {
            lat: Number(this.lsL0Element.value) * Math.PI / 180,
            lst: Number(this.lsLSTElement.value) * Math.PI / 180,
            alt: Number(this.lsAltElement.value),
        }

        this.calculate();
    }

    calculate() {
        if (this.orbit && this.launchSite) {
            this.launchRequirementCalculator = new Module.LaunchRequirementCalculator(this.orbit, this.launchSite);
            this.launchRequirementCalculator.calculateRequiredDeltaV(Number(this.deltaVLossesElement.value || 0));
            this.render();
        }
    }

    render() {
        this.vLSElement.innerHTML = printVector(this.launchRequirementCalculator.vLS, 'SEZ') + ' km/s';
        this.vLGElement.innerHTML = printVector(this.launchRequirementCalculator.vLG, 'SEZ') + ' km/s';
        this.vBOElement.innerHTML = printVector(this.launchRequirementCalculator.vBO, 'SEZ') + ' km/s';
        this.deltaVNeededElement.innerHTML = fixRoundingError(this.launchRequirementCalculator.deltaVNeeded || 0) + ' km/s';
        this.deltaVDesignElement.innerHTML = fixRoundingError(this.launchRequirementCalculator.deltaVDesign || 0) + ' km/s';
    }
}

window.customElements.define('launch-requirement-calculator', LaunchRequirementCalculator);