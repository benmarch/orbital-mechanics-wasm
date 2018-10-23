import Component, { html } from '../Component.js';
import './Button.js'

class Header extends Component {
    static get template() {
        return html`
            <style>
                :host {
                    display: block;
                    width: 100%;
                }

                header {
                    border-bottom: 1px solid #aaa;
                    display: flex;
                    align-items: center;
                }

                #mainMenuButton {
                    align-self: flex-start;
                    margin: 10px;
                    z-index: 2;
                }

                .bar {
                    display: block;
                    width: 15px;
                    height: 2px;
                    margin: 2px 0;
                    border-radius: 1px;
                    background-color: #aaa;
                }

                #title {
                    position: absolute;
                    left: 0;
                    width: 100%;
                    text-align: center;
                    font-size: var(--font-size-large);
                }
            </style>

            <header>
                <om-button id="mainMenuButton" on:click="navigateToMainMenu">
                    <div class="bar"></div>
                    <div class="bar"></div>
                    <div class="bar"></div>
                </om-button>
                <div id="title">
                    Intro to Orbital Mechanics
                </div>
            </header>
        `;
    }

    navigateToMainMenu() {
        window.location = '#mainmenu';
    }
}

window.customElements.define('om-header', Header);