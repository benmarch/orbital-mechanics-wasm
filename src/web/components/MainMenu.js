import Component, { html } from '../Component.js';

class MainMenu extends Component {
    static get template() {
        return html`
            <style>
                :host {
                    display: block;
                    width: 100%;
                }

                #menu {
                    display: block;
                }

                ul {
                    padding-left: 0;
                }

                li {
                    font-size: var(--font-size-large);
                    list-style-type: none;
                    text-align: center;
                }

                a {
                    color: inherit;
                    text-decoration: none;
                }

                a:hover {
                    text-decoration: underline;
                }
            </style>

            <nav id="menu">
                <ul>
                    <li><a href="#myorbits">My Orbits</a></li>
                    <li><a href="#createorbit">Create an Orbit</a></li>
                    <li><a href="#hohmanntransfer">Calculate Hohmann Transfer</a></li>
                    <li>Calculate Plane Change</li>
                    <li>Calculate Bi-elliptic Transfer</li>
                    <li>Patched Conic Approximation</li>
                </ul>
            </nav>
        `;
    }
}

window.customElements.define('main-menu', MainMenu);