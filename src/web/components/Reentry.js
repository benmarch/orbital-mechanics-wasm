import Component, {html} from '../Component.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './TextInput.js';

class Reentry extends Component {
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

                .data-container {
                    width: 700px;
                }
            </style>

            <dl class="data-container">
                <dt>Reentry Velocity (km/s):</dt>
                <dd><text-input type="number" id="reentryVelocity" on:input="handleChange"></text-input></dd>

                <dt>Flightpath Angle (degrees):</dt>
                <dd><text-input type="number" id="flightpathAngle" on:input="handleChange"></text-input></dd>

                <dt>Ballistic Coefficient (kg/m<sup>2</sup>):</dt>
                <dd><text-input type="number" id="ballisticCoefficient" on:input="handleChange"></text-input></dd>
            </dl>

            <dl class="data-container">
                <dt>Maximum Deceleration (km/s<sup>2</sup>) = </dt>
                <dd id="maxDeceleration"></dd>

                <dt>Altitude at Maximum Deceleration (km) = </dt>
                <dd id="altitudeMaxDecleration"></dd>

                <dt>Altitude at Maximum Heating Rate (km) = </dt>
                <dd id="altitudeMaxHeating"></dd>

                <dt>Velocity at Maximum Heating Rate (km/s) = </dt>
                <dd id="velocityMaxHeating"></dd>
            </dl>
        `;
    }

    constructor() {
        super();

        this.reentry = new Module.Reentry();
    }

    handleChange() {
        this.reentryVelocity = Number(this.reentryVelocityElement.value || 0);
        this.flightpathAngle = Number(this.flightpathAngleElement.value || 0) * Math.PI / 180;
        this.ballisticCoefficient = Number(this.ballisticCoefficientElement.value || 0);

        this.calculate();
    }

    calculate() {
        if (this.reentryVelocity && this.flightpathAngle && this.ballisticCoefficient) {
            this.reentry.calculateFromReentryConditions(this.reentryVelocity, this.flightpathAngle, this.ballisticCoefficient);
            this.render();
        }
    }

    render() {
        this.maxDecelerationElement.innerHTML = fixRoundingError(this.reentry.maxDeceleration) + ' km/s<sup>2</sup>';
        this.altitudeMaxDeclerationElement.innerHTML = fixRoundingError(this.reentry.altitudeMaxDeceleration) + ' km';
        this.altitudeMaxHeatingElement.innerHTML = fixRoundingError(this.reentry.altitudeMaxHeating) + ' km';
        this.velocityMaxHeatingElement.innerHTML = fixRoundingError(this.reentry.velocityMaxHeating) + ' km/s';
    }
}

window.customElements.define('reentry-calculator', Reentry);