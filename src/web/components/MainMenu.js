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
                    <li><a href="#planechange">Calculate Plane Change</a></li>
                    <li><a href="#coplanarrendezvous">Calculate Coplanar Rendezvous</a></li>
                    <li><a href="#coorbitalrendezvous">Calculate Coorbital Rendezvous</a></li>
                    <li><a href="#pca">Patched Conic Approximation</a></li>
                    <li><a href="#tof">Calculate Time of Flight</a></li>
                    <li><a href="#j2">Calculate J2 Effects (Earth Only)</a></li>
                    <li><a href="#launchwindow">Calculate Launch Window</a></li>
                    <li><a href="#launchreq">Calculate Launch Requirements</a></li>
                    <li><a href="#reentry">Calculate Reentry Conditions</a></li>
                    <li><a href="#thrust">Thrust Calculation Tools</a></li>
                    <li><a href="#staging">Rocket Staging</a></li>
                </ul>
            </nav>
        `;
    }
}

window.customElements.define('main-menu', MainMenu);