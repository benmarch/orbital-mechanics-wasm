import Component, {html} from '../Component.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './TextInput.js';
import './Button.js';

class ThrustCalculationTools extends Component {
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

            <section id="jetPowerSection">
                <header><h3>Calculate Jet Power</h3></header>

                P<sub>J</sub> = &frac12;
                    <text-input id="jetPowerMassFlowRate" placeholder="Mass Flow Rate (kg/s)"></text-input> &times;
                    <text-input id="jetPowerEffectiveExhaustVelocity" placeholder="Effective Exhaust Velocity (m/s)"></text-input> =
                    <span id="jetPower">0</span> W

                <om-button on:click="calculate"
                           data-answer-element-id="jetPower"
                           data-method-name="calculateJetPowerFromMassFlowRateAndEffectiveExhaustVelocity"
                           data-argument-element-ids="jetPowerMassFlowRate, jetPowerEffectiveExhaustVelocity">Calculate</om-button>
            </section>

            <section id="thrustSection">
                <header><h3>Calculate Thrust</h3></header>

                F<sub>Thrust</sub> =
                    <text-input id="thrust1MassFlowRate" placeholder="Mass Flow Rate (kg/s)"></text-input> &times;
                    <text-input id="thrust1EffectiveExhaustVelocity" placeholder="Effective Exhaust Velocity (m/s)"></text-input> =
                    <span id="thrust1">0</span> N

                <om-button on:click="calculate"
                           data-answer-element-id="thrust1"
                           data-method-name="calculateThrustFromMassFlowRateAndEffectiveExhaustVelocity"
                           data-argument-element-ids="thrust1MassFlowRate, thrust1EffectiveExhaustVelocity">Calculate</om-button>

                <br>

                F<sub>Thrust</sub> =
                    <text-input id="thrust2DeltaMomentum" placeholder="∆p (kg m/s)"></text-input> &divide;
                    <text-input id="thrust2DeltaTime" placeholder="∆t (s)"></text-input> =
                    <span id="thrust2">0</span> N

                <om-button on:click="calculate"
                           data-answer-element-id="thrust1"
                           data-method-name="calculateThrustFromDeltaMomentumAndDeltaTime"
                           data-argument-element-ids="thrust2DeltaMomentum, thrust2DeltaTime">Calculate</om-button>
            </section>

            <section id="impulseSection">
                <header><h3>Calculate Impulse</h3></header>

                I =
                    <text-input id="impulse1Thrust" placeholder="Thrust (N)"></text-input> &times;
                    <text-input id="impulse1DeltaTime" placeholder="∆t (s)"></text-input> =
                    <span id="impulse1">0</span> N s

                <om-button on:click="calculate"
                           data-answer-element-id="impulse1"
                           data-method-name="calculateImpulseFromThrustAndDeltaTime"
                           data-argument-element-ids="impulse1Thrust, impulse1DeltaTime">Calculate</om-button>
            </section>

            <section id="specificImpulseSection">
                <header><h3>Calculate Specific Impulse</h3></header>

                I<sub>sp</sub> =
                    <text-input id="isp1Impulse" placeholder="Impulse (N s)"></text-input> &divide;
                    (<text-input id="isp1PropellantMass" placeholder="Propellant Mass (kg)"></text-input> &times; g<sub>0</sub>) =
                    <span id="isp1">0</span> s<sup>-1</sup>

                <om-button on:click="calculate"
                           data-answer-element-id="isp1"
                           data-method-name="calculateSpecificImpulseFromImpulseAndPropellantMass"
                           data-argument-element-ids="isp1Impulse, isp1PropellantMass">Calculate</om-button>

                <br>

                I<sub>sp</sub> =
                    <text-input id="isp2Thrust" placeholder="Thrust (N)"></text-input> &divide;
                    (<text-input id="isp2MassFlowRate" placeholder="Mass Flow Rate (kg/s)"></text-input> &times; g<sub>0</sub>) =
                    <span id="isp2">0</span> s<sup>-1</sup>

                <om-button on:click="calculate"
                           data-answer-element-id="isp2"
                           data-method-name="calculateSpecificImpulseFromThrustAndMassFlowRate"
                           data-argument-element-ids="isp2Thrust, isp2MassFlowRate">Calculate</om-button>

                <br>

                I<sub>sp</sub> =
                    <text-input id="isp3EffectiveExhaustVelocity" placeholder="Effective Exhaust Velocity (m/s)"></text-input> &divide; g<sub>0</sub> =
                    <span id="isp3">0</span> s<sup>-1</sup>

                <om-button on:click="calculate"
                           data-answer-element-id="isp3"
                           data-method-name="calculateSpecificImpulseFromEffectiveExhaustVelocity"
                           data-argument-element-ids="isp3EffectiveExhaustVelocity">Calculate</om-button>
            </section>

            <section id="deltaVSection">
                <header><h3>Calculate ∆V</h3></header>

                ∆V =
                    <text-input id="deltaV1EffectiveExhaustVelocity" placeholder="Effective Exhaust Velocity (m/s)"></text-input> &times; ln(
                    <text-input id="deltaV1InitialMass" placeholder="Initial Mass (kg)"></text-input> &divide;
                    <text-input id="deltaV1FinalMass" placeholder="Final Mass (kg)"></text-input>) =
                    <span id="deltaV1">0</span> m/s

                <om-button on:click="calculate"
                           data-answer-element-id="deltaV1"
                           data-method-name="calculateDeltaVUsingEffectiveExhaustVelocity"
                           data-argument-element-ids="deltaV1EffectiveExhaustVelocity, deltaV1InitialMass, deltaV1FinalMass">Calculate</om-button>

                <br>

                ∆V =
                    <text-input id="deltaV2SpecificImpulse" placeholder="Specific Impulse (s<sup>-1</sup>)"></text-input> &times; g<sub>0</sub> &times ln(
                    <text-input id="deltaV2InitialMass" placeholder="Initial Mass (kg)"></text-input> &divide;
                    <text-input id="deltaV2FinalMass" placeholder="Final Mass (kg)"></text-input>) =
                    <span id="deltaV2">0</span> m/s

                <om-button on:click="calculate"
                           data-answer-element-id="deltaV2"
                           data-method-name="calculateDeltaVUsingSpecificImpulse"
                           data-argument-element-ids="deltaV2SpecificImpulse, deltaV2InitialMass, deltaV2FinalMass">Calculate</om-button>
            </section>

            <section id="massSection">
                <header><h3>Calculate Mass</h3></header>

                mass<sub>propellant used</sub> =
                    <text-input id="mass1InitialMass" placeholder="Initial Mass (kg)"></text-input> -
                    m<sub>i</sub> &divide; e<sup>(</sup>
                    <text-input id="mass1DeltaV" placeholder="∆V (m/s)"></text-input> &divide;
                    <text-input id="mass1EffectiveExhaustVelocity" placeholder="Effective Exhaust Velocity (m/s)"></text-input> <sup>)</sup>
                     =
                    <span id="mass1">0</span> kg

                <om-button on:click="calculate"
                           data-answer-element-id="mass1"
                           data-method-name="calculatePropellantMassFromDeltaVAndEffectiveExhaustVelocityAndInitialMass"
                           data-argument-element-ids="mass1DeltaV, mass1EffectiveExhaustVelocity, mass1InitialMass">Calculate</om-button>
            </section>
        `;
    }

    constructor() {
        super();

        this.thrustCalculator = new Module.ThrustCalculator();
    }

    calculate(event) {
        const answerElementId = event.target.dataset.answerElementId;
        const methodName = event.target.dataset.methodName;
        const argumentElementIds = event.target.dataset.argumentElementIds.split(',').map(id => id.trim());

        this[answerElementId + 'Element'].innerHTML = this.thrustCalculator[methodName](...argumentElementIds.map(id => Number(this[id + 'Element'].value || 0)));
    }
}

window.customElements.define('thrust-calculation-tools', ThrustCalculationTools);