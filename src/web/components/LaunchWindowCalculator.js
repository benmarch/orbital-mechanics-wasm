import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';
import {fixRoundingError} from '../utils.js';

import './OrbitSelector.js';
import './TextInput.js';
import './Button.js';

class LaunchWindowCalculator extends Component {
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

                .is-hidden,
                .is-next-label {
                    display: none;
                }

                .is-next .is-next-label {
                    display: inline;
                }
            </style>

            <div id="left">
                <div id="orbitSelect">
                    <h3>Please select an orbit: </h3>
                    <div id="errorMessage"></div>
                    <orbit-selector max-orbits="1" on:orbitselected="handleOrbitSelected"></orbit-selector>
                    <div id="selectedOrbits"></div>
                </div>

                <section id="launchSite">
                    <h4>Launch Site:</h4>
                    <dl>
                        <dt>L<sub>0</sub> (degrees)</dt>
                        <dd><text-input type="number" id="lsL0"></text-input></dd>

                        <dt>LST (degrees)</dt>
                        <dd><text-input type="number" id="lsLST"></text-input></dd>

                        <dt>Altitude (km)</dt>
                        <dd><text-input type="number" id="lsAlt"></text-input></dd>
                    </dl>
                    <om-button on:click="handleLaunchSiteSet">Calculate</om-button>
                </section>
            </div>
            <div id="right">
                <h4 id="dataTitle">Data:</h4>
                <section id="onlyOpportunity" class="is-hidden">
                    <header><h3>Only Opportunity</h3></header>
                    <dl>
                        <dt>Alpha</dt>
                        <dd id="oAlpha"></dd>

                        <dt>Gamma</dt>
                        <dd id="oGamma"></dd>

                        <dt>Delta</dt>
                        <dd id="oDelta"></dd>

                        <dt>Beta</dt>
                        <dd id="oBeta"></dd>

                        <dt>LWST</dt>
                        <dd id="oLWST"></dd>

                        <dt>Wait Time (h)</dt>
                        <dd id="oWaitTime"></dd>
                    </dl>
                </section>

                <section id="ascendingNodeOpportunity" class="is-hidden">
                    <header><h3>Ascending Node Opportunity <span class="is-next-label">(Next)</span></h3></header>
                    <dl>
                        <dt>Alpha</dt>
                        <dd id="anAlpha"></dd>

                        <dt>Gamma</dt>
                        <dd id="anGamma"></dd>

                        <dt>Delta</dt>
                        <dd id="anDelta"></dd>

                        <dt>Beta</dt>
                        <dd id="anBeta"></dd>

                        <dt>LWST</dt>
                        <dd id="anLWST"></dd>

                        <dt>Wait Time (h)</dt>
                        <dd id="anWaitTime"></dd>
                    </dl>
                </section>

                <section id="descendingNodeOpportunity" class="is-hidden">
                    <header><h3>Descending Node Opportunity <span class="is-next-label">(Next)</span></h3></header>
                    <dl>
                        <dt>Alpha</dt>
                        <dd id="dnAlpha"></dd>

                        <dt>Gamma</dt>
                        <dd id="dnGamma"></dd>

                        <dt>Delta</dt>
                        <dd id="dnDelta"></dd>

                        <dt>Beta</dt>
                        <dd id="dnBeta"></dd>

                        <dt>LWST</dt>
                        <dd id="dnLWST"></dd>

                        <dt>Wait Time (h)</dt>
                        <dd id="dnWaitTime"></dd>
                    </dl>
                </section>
            </div>
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
            this.launchWindowCalculator = new Module.LaunchWindowCalculator(this.orbit, this.launchSite);
            this.render();
        }
    }

    render() {
        this.onlyOpportunityElement.classList.add('is-hidden');
        this.ascendingNodeOpportunityElement.classList.add('is-hidden');
        this.descendingNodeOpportunityElement.classList.add('is-hidden');

        if (this.launchWindowCalculator.numLaunchOpportunities === 1) {
            this.onlyOpportunityElement.classList.remove('is-hidden');

            this.oAlphaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.onlyOpportunity.alpha * 180 / Math.PI);
            this.oGammaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.onlyOpportunity.gamma * 180 / Math.PI);
            this.oDeltaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.onlyOpportunity.delta * 180 / Math.PI);
            this.oBetaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.onlyOpportunity.beta * 180 / Math.PI);
            this.oLWSTElement.innerHTML = fixRoundingError(this.launchWindowCalculator.onlyOpportunity.lwst * 180 / Math.PI);
            this.oWaitTimeElement.innerHTML = fixRoundingError(this.launchWindowCalculator.onlyOpportunity.waitTime);
        } else {
            this.ascendingNodeOpportunityElement.classList.remove('is-hidden');
            this.descendingNodeOpportunityElement.classList.remove('is-hidden');
            if (this.launchWindowCalculator.ascendingNodeOpportunity.isNext) {
                this.ascendingNodeOpportunityElement.classList.add('is-next');
            }
            if (this.launchWindowCalculator.descendingNodeOpportunity.isNext) {
                this.descendingNodeOpportunityElement.classList.add('is-next');
            }

            this.anAlphaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.ascendingNodeOpportunity.alpha * 180 / Math.PI);
            this.anGammaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.ascendingNodeOpportunity.gamma * 180 / Math.PI);
            this.anDeltaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.ascendingNodeOpportunity.delta * 180 / Math.PI);
            this.anBetaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.ascendingNodeOpportunity.beta * 180 / Math.PI);
            this.anLWSTElement.innerHTML = fixRoundingError(this.launchWindowCalculator.ascendingNodeOpportunity.lwst * 180 / Math.PI);
            this.anWaitTimeElement.innerHTML = fixRoundingError(this.launchWindowCalculator.ascendingNodeOpportunity.waitTime);

            this.dnAlphaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.descendingNodeOpportunity.alpha * 180 / Math.PI);
            this.dnGammaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.descendingNodeOpportunity.gamma * 180 / Math.PI);
            this.dnDeltaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.descendingNodeOpportunity.delta * 180 / Math.PI);
            this.dnBetaElement.innerHTML = fixRoundingError(this.launchWindowCalculator.descendingNodeOpportunity.beta * 180 / Math.PI);
            this.dnLWSTElement.innerHTML = fixRoundingError(this.launchWindowCalculator.descendingNodeOpportunity.lwst * 180 / Math.PI);
            this.dnWaitTimeElement.innerHTML = fixRoundingError(this.launchWindowCalculator.descendingNodeOpportunity.waitTime);
        }
    }
}

window.customElements.define('launch-window-calculator', LaunchWindowCalculator);