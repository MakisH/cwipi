---
title: 'CWIPI: Coupling With Interpolation Parallel Interface'
tags:
  - parallel computing
  - coupling
  - multi-physics
  - MPI

authors:
  - name: Eric Quémerais
    orcid: 0009-0007-2018-6358
    affiliation: "1"

  - name: Bastien Andrieu
    orcid: 0009-0000-9937-0244
    affiliation: "1"

  - name: Karmijn Hoogveld
    orcid: 0009-0007-8251-1152
    affiliation: "1"

affiliations:
 - index: 1
   name: DMPE, ONERA, Université Paris-Saclay, 92320 Châtillon, France

date: 27 January 2026

bibliography: paper.bib

---

# Summary

CWIPI is an open-source library for the parallel coupling of independent simulation codes in multi-physics research. It provides MPI-based communication and fully distributed data exchange on non-coincident meshes, including on-the-fly spatial interpolation. Unlike other frameworks such as MUI [@Tang2015] and preCICE [@Chourdakis2022], CWIPI focuses on spatial interpolation and allows users to leverage solver-specific numerical methods, including finite volumes, finite elements, discontinuous Galerkin, and spectral methods. Advanced users can combine CWIPI's geometric algorithms with solvers' internal interpolation through callbacks, improving accuracy across heterogeneous solvers.

CWIPI supports a wide range of interface geometries, from linear, surface and volumetric elements to higher-order elements with curved edges and faces. Performance-critical algorithms have been optimized for dynamic and adaptive meshes. Recent developments in the ParaDiGM library [@Quemerais2026] provide dynamic load and memory balancing for large-scale simulations. Ongoing work includes GPU acceleration of geometric algorithms [@Cazalbou2024] and the development of complementary temporal interpolation methods [@Simon2025].

# Statement of Need

Multi-physics problems can be approached either through monolithic methods — where all physical phenomena are solved simultaneously within a single solver — or through partitioned methods, where independent solvers exchange data across shared interfaces. Partitioned approaches are particularly attractive in high-performance computing (HPC) contexts, as they allow the reuse of existing specialized codes while maintaining their native parallel architecture.

Robust and efficient coupling tools are essential to enable data exchange between solvers operating on distinct meshes, geometries, and discretization schemes. CWIPI was initially developed in 2009 to provide an open-source solution complementary to MPCCI [@Joppich2006], whose black-box nature limited flexibility and transparency for multi-physics research.

A key strength of CWIPI lies in its ability to **adapt to solvers' own numerical methods** — finite volumes, finite elements, discontinuous Galerkin, or spectral methods — and to leverage their internal interpolation capabilities. In an advanced mode, CWIPI exposes the results of its geometric algorithms, allowing users to combine them with the solver's internal methods via a *callback*. For example, it is possible to use the functional bases of high-order elements from the source mesh while knowing the coordinates of target points from a finite-difference solver [@Leger2012], improving accuracy between heterogeneous solvers while maintaining consistency with each solver's numerical scheme.

# State of the Field

Existing coupling frameworks differ significantly in scope, geometry support, and interpolation capabilities. MUI [@Tang2015] primarily supports point clouds and focuses on temporal and spatial interpolation in a general-purpose fashion. preCICE [@Chourdakis2022] supports point clouds or meshes composed of planar triangular or quadrilateral elements and provides a broad ecosystem of adapters for established simulation codes. OpenPALM [@Duchaine2015] is a higher-level coupler that relies on CWIPI as its interpolation engine.

CWIPI addresses several limitations of these frameworks. It supports a broader range of interface representations, including linear, surface and volumetric elements, polygons and polyhedra, as well as higher-order elements with curved edges and faces. Where preCICE [@Chourdakis2022] and MUI [@Tang2015] focus on generic coupling workflows, CWIPI focuses primarily on spatial interpolation fidelity, leaving temporal interpolation strategies to the user's implementation — an approach well suited to research contexts where solver-specific accuracy is paramount.

# Software Design

Since version 1.0, CWIPI is built on top of the ParaDiGM library [@Quemerais2026], originally created specifically to meet the needs of CWIPI. ParaDiGM provides the low-level geometric algorithms and parallel data structures. The coupling workflow is organized around three main components: geometric localization (finding which source mesh element contains each target point), data redistribution (MPI-based parallel exchange of located data), and interpolation (computing field values at target locations from source element data).

CWIPI provides three APIs — C/C++, Fortran, and Python/NumPy — making it compatible with a wide range of scientific codes. The integration of CWIPI into an existing solver is designed to be **minimally intrusive**: it does not require modifying the code structure, and the API relies solely on simple arrays, keeping the coupling layer lightweight and straightforward to adopt.

A major design challenge lies in the **performance of geometric algorithms**, especially when the coupling interface moves during the simulation or when solvers employ dynamic mesh adaptation. In the 0.x branch of CWIPI, algorithms relied on the FVM library from Code_Saturne [@Archambeau2004], which later evolved into PLE, a lightweight coupling library [@Fournier2020]. New algorithms [@Andrieu2026] developed within ParaDiGM provide **dynamic load and memory balancing** at each coupling step, which is critical because coupling problems are inherently unbalanced across MPI processes. These developments bring significant performance gains for large-scale and complex simulations.

## Future Directions

Two main research directions are currently pursued. The first concerns the **GPU porting** of the most computationally expensive geometric algorithms, for which new algorithms have recently been designed [@Cazalbou2024] and will be integrated in an upcoming release. The second focuses on the development of **temporal interpolation methods** [@Simon2025; @Francois2023], complementary to those provided by existing frameworks such as preCICE, aiming to improve accuracy and flexibility for dynamic interfaces and heterogeneous solvers.

# Research Impact Statement

From a scientific perspective, CWIPI is primarily used within the aeronautics and aerospace community from which it originated. The library is integrated into several codes developed at ONERA, including elsA [@Cambier2013] for computational fluid dynamics, CEDRE [@Refloch2011] for multiphysics simulations, Z-set [@Garaud2019] for structural and materials mechanics, MoDeTheC [@Dellinger2024] for thermal degradation of materials, and SPACE [@Delorme2005] for acoustic propagation. Through these tools, CWIPI enables advanced multiphysics simulations in aerothermodynamics [@Errera2019], aeroacoustics [@Langenais2019], magnetohydrodynamics [@Rocamora2025], electric arc propagation, and fire safety [@Dellinger2023].

CWIPI is also employed as both an internal and external coupling library in leading combustion simulation environments such as YALES2 [@Moureau2011] and AVBP [@Schonfeld1999]. A notable HPC application [@Dombard2018] combining CWIPI with AVBP on a complex geometry demonstrated the robustness and scalability of the coupling approach on massively parallel architectures. Its capabilities were highlighted as early as 2017 [@Duchaine2017] and confirmed by an invitation to the ExCALIBUR workshop [@Andrieu2021]. Recent work in aeroelasticity with YALES2 [@Fabbri2023] further demonstrates its versatility. CWIPI's integration into the software stack of national HPC facilities such as the CCRT confirms its recognition at a national level.

Beyond its original ecosystem, the open-source community has adopted CWIPI through GitHub-based interfacing projects with OpenFOAM [@Weller1998; @MoratillaVega2022; @nd922025] and Nektar++ [@Cantwell2015], enabling applications in aeroacoustics and data assimilation [@Valero2026]. Finally, CWIPI serves as the interpolation engine of the OpenPALM coupler [@Duchaine2015], illustrating its central role in the broader multiphysics and HPC coupling ecosystem.

# AI Usage Disclosure

Generative AI tools were used to assist with the writing of this paper: translation, formatting, and improving the fluency of the text. No AI was used in the development of the CWIPI software. All scientific content was written and validated by the authors.

# Acknowledgements

The authors would like to thank the following people for their contributions to the development, testing, and dissemination of the CWIPI library within the community:

Romain Casta¹, Nicolas Dellinger², Florent Duchaine¹, Bastien Frisulli², Jean-Didier Garaud², Thomas Hennion², Stéphanie Lala², Xavier Lamboley¹², Bruno Maugars², Thierry Morel¹, Christophe Peyret²

¹ CERFACS — ² ONERA

The authors also wish to thank BPI France, the Directorate General for Civil Aviation (DGAC), and the General Scientific Directorate of ONERA for their financial support.

# Author Contributions

The contributions to this software are listed according to the CRediT taxonomy:

- **Eric Quémerais**: Conceptualization, Methodology, Software, Validation, Writing – Original Review & Editing, Project Administration, Funding Acquisition, Supervision
- **Bastien Andrieu**: Software, Validation, Writing – Original Draft
- **Karmijn Hoogveld**: Software, Validation, Writing – Original Draft

# References
